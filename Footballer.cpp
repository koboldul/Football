#include "pch.h"

#include "Footballer.h"
#include "StadionLayer.h"
#include "SoccerGlobal.h"

using namespace cocos2d;
using namespace SoccerGlobal;

Footballer::~Footballer()
{
    CCLog("destroy footballer");
}

Footballer::Footballer(StadionLayer * game, int type, CCPoint position) : b2Sprite (game, kSpriteFootballer) {
    _type = type;
	_startPosition = position;
}

Footballer* Footballer::create(StadionLayer * game, int type, CCPoint position) {
	Footballer * sprite = new Footballer(game, type, position);
	
	if (sprite) {
        sprite->initFootballer(game);
		sprite->autorelease();
		
		return sprite;
	}
	
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Footballer::initFootballer(StadionLayer * game) {
    
    b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	
	_body = game->getWorld()->CreateBody(&bodyDef);
    
    //create circle shape
    b2CircleShape  circle;
    circle.m_radius = PLAYER_RADIUS/PTM_RATIO;
    
    //define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 7;
    fixtureDef.restitution = 0.5;
    
	_body->CreateFixture(&fixtureDef);
    //CCTintTo * c =  CCTintTo::create(0,23,239,23);
   // this->runAction(c);

	if ( kLeft == _type)
	{
		initWithSpriteFrameName("white_shadow.png");
	}
	else
	{
		initWithSpriteFrameName("orange_shadow.png");
        setRotation(180);
	}
    
    Settings::getInstance().scaleSprite(this);
	_body->SetUserData(this);
    
    
    setSpritePosition(_startPosition);

	CCSize ss = boundingBox().size;
}
