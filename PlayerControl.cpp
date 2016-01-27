#include "pch.h"

#include "PlayerControl.h"

PlayerControl::PlayerControl(int id) : _time(0), _id(id)
{
    _state.isInitialized = true;
    _state.isActive = false;
    _state.hasShoot = false;
    _state.isHost = false;
    _state.localConnectionEstablished = false;
    _state.remoteConnectionEstablished = false;
    _state.needsSync = false;
    
    setPlayerType(kHuman);
    
}

PlayerControl::~PlayerControl()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, START_SHOOT);
    CCLog("Destroyng control for player");
}


void PlayerControl::init()
{
    std:string name = CCUserDefault::sharedUserDefault()->getStringForKey(USER_DATA_NAME);
    initWithPlayerName(name);
   
}

void PlayerControl::initWithPlayerName(string name)
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PlayerControl::startShooting), START_SHOOT, NULL);
    setPlayerName(name);
   
};

void  PlayerControl::startShooting(ObserverData* param)
{
    if (getIsActive()) 
    {
        CCPoint initialPos = param->getInitialPosition();
        CCPoint finalPos = param->getFinalPosition();
    
        double angle = atan2f(finalPos.y-initialPos.y, finalPos.x - initialPos.x);
	
        float powa = ccpDistance(initialPos, finalPos);
        b2Vec2 speed = b2Vec2(powa * cos(angle), powa * sin(angle));
    
        ObserverData* data = new ObserverData();
        data->setSpeed(speed);
    
        CCNotificationCenter::sharedNotificationCenter()->postNotification(RESOLVE_SHOOT, data);
        
        _state.hasShoot = false;
    }
}

void PlayerControl::setIsActive(bool isActive)
{
    _state.isActive = isActive;
    
    if (_state.isActive)
    {
        CCLog("Player %s is active" , getPlayerName().c_str());
        _state.hasShoot = true;
    }
    else
    {
        _state.hasShoot = false;
    }
}

bool PlayerControl::getIsActive()
{
    return _state.isActive;
}

void PlayerControl::update(CCPoint pos, b2Vec2 speed)
{
}

void PlayerControl::gameQuit(){}
