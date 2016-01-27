#include "pch.h"
#include "Ball.h"
#include "StadionLayer.h"

#define BALL_ANIMATION_SPEED 3.5

Ball::~Ball()
{
    CC_SAFE_RELEASE(_animateBall);
    CCLog("destroy ball");
}

Ball::Ball (StadionLayer * game, CCPoint position) : b2Sprite (game, kSpriteBall) {
    _startPosition = position;
}

Ball* Ball::create(StadionLayer * game, CCPoint position) {
    Ball * sprite = new Ball(game, position);
	if (sprite) {
        sprite->initBall();
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Ball::initBall() {
    //create box2d body
    b2BodyDef bodyDef;
	
	bodyDef.type = b2_dynamicBody;
	
	_body = getGame()->getWorld()->CreateBody(&bodyDef);
    
    _body->SetLinearDamping(1.2);
    _body->SetAngularDamping(1.2);
    
    //create circle shape
    b2CircleShape  circle;
    circle.m_radius = BALL_RADIUS/PTM_RATIO;
    
    //define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 7;
    fixtureDef.restitution = 0.65;
    
    _body->SetBullet(true);
    _body->CreateFixture(&fixtureDef);
    
	initWithSpriteFrameName("ball1.png");
    
	_body->SetUserData(this);
    
    Settings::getInstance().scaleSprite(this, ballRatio);
    setSpritePosition(_startPosition);
    
       
    CCAnimation* animation;
    CCSpriteFrame * frame;
    
    //create CCAnimation object
    animation = CCAnimation::create();
    CCString * name;
    for(int i = 7; i >=  1; i--) {
        name = CCString::createWithFormat("ball%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animation->addSpriteFrame(frame);
    }
    animation->setDelayPerUnit(0.5f);
    
    _animateBall = CCSpeed::create(CCRepeatForever::create(CCAnimate::create(animation)), 4.5f);
    
    _animateBall->retain();
}

void Ball::startShooting(b2Vec2 velocity)
{
    _body->ApplyLinearImpulse(velocity, _body->GetWorldCenter());
    
    _animateBall->setSpeed(velocity.Length()* BALL_ANIMATION_SPEED) ;
    stopAllActions();
    runAction(_animateBall);
}

void Ball::reset()
{
    if (_body) {
       _body->SetLinearVelocity(b2Vec2_zero);
       _body->SetAngularVelocity(0);
       _body->SetAwake(true);
    }
    stopAllActions();
    setSpritePosition(_startPosition);
    setVisible(true);
}

void Ball::update (float dt)
{
    {
        if (_body && isVisible())
		{
			setPositionX(_body->GetPosition().x * PTM_RATIO);
			setPositionY(_body->GetPosition().y * PTM_RATIO);
        
			float speed = _body->GetLinearVelocity().Length() * 0.7f;
        
			if (speed < 0.3)
			{
				_body->SetLinearVelocity(b2Vec2(0.0, 0.0));
				speed = 0;
			}
        
			_animateBall->setSpeed(speed * BALL_ANIMATION_SPEED);
        }
    }
}