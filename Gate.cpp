#include "pch.h"

#include "Gate.h"
#include "SoccerGlobal.h"

using namespace cocos2d;
using namespace SoccerGlobal;

Gate::~Gate() {   
}

Gate::Gate(StadionLayer * game, int type) : b2Sprite (game, kSpriteGate) {
    _type = type;
    setHoldsBall(false);
}

Gate* Gate::create(StadionLayer * game, int type) {
    Gate * sprite = new Gate(game, type);
	
	if (sprite) {
        sprite->initGate(game);
		sprite->autorelease();
		
		return sprite;
	}
	
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Gate::initGate(StadionLayer * game) {
    
    //create box2d body
    b2BodyDef bodyDef;
	
	bodyDef.type = b2_staticBody;
	
	_body = _game->getWorld()->CreateBody(&bodyDef);
    
    if ( kLeft == _type)
	{
		initWithSpriteFrameName("gateLeft.png");
		_startPosition = ccp(Settings::getInstance().getXOffset() + MARGIN_TICKNESS - getContentSize().width/2,
							 Settings::getInstance().getYOffset() + MARGIN_TICKNESS + game->getTerrainHeight() /2);
	}
	else
	{
		initWithSpriteFrameName("gateRight.png");
		_startPosition = ccp(Settings::getInstance().getXOffset() + MARGIN_TICKNESS + game->getTerrainWidth() + getContentSize().width/2,
							 Settings::getInstance().getYOffset() + MARGIN_TICKNESS + game->getTerrainHeight() /2);
	}

    
    //create circle shape
	b2PolygonShape square;
	square.SetAsBox(25 * 0.5f / PTM_RATIO,  120 * 0.5f / PTM_RATIO);
    
    //define fixture
    b2FixtureDef fixtureDef;
	fixtureDef.shape = &square;
	fixtureDef.isSensor = true;
    
    _body->CreateFixture(&fixtureDef);
    
    setSpritePosition(_startPosition);
    Settings::getInstance().scaleSprite(this);
    
    _body->SetUserData(this);
}
