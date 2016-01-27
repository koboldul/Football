/*
 * Defines the actual player - computer, human.	
 */

#ifndef __PlayerControl__
#define __PlayerControl__

#include "SoccerGlobal.h"
#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "Box2D/Box2D.h"
#else
#include "Box2D/Box2D.h"
#endif
#include "ObserverData.h"

using namespace SoccerGlobal;
using namespace cocos2d;

struct ControlState
{
    bool isActive;
    bool hasShoot;
    bool isInitialized;
    bool isHost ;
    bool localConnectionEstablished ;
    bool remoteConnectionEstablished;
    bool needsSync;
} ;


class PlayerControl : public CCObject
{

public:
    ~PlayerControl();
	PlayerControl(int id);
	
	virtual void init();
    virtual void initWithPlayerName(string name);
	virtual bool whenActiveTouchAllowed()
    {
        return true;
    }

    virtual void update(CCPoint pos, b2Vec2 speed);
    virtual void setIsActive(bool isActive);
    virtual void gameQuit();
    
    bool getIsActive();
    
    CC_SYNTHESIZE(ControlState, _state, State);
    CC_SYNTHESIZE(OpponentType, _opType, PlayerType);
    CC_SYNTHESIZE(string, _playerName, PlayerName);
    CC_SYNTHESIZE(int, _terrainSide, Side);
    CC_SYNTHESIZE(int, _id, ID)
    
    int getTime()
    {
        //if (getIsActive())
        {
            _time++;
        }
        
        return _time;
    }
    
    
protected:
    virtual void  startShooting(ObserverData* param);
    
    
private:
    int _time;
    
};

#endif /* defined(__PlayerControl__) */