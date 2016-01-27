 //
//  GameConfiguration.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/22/13.
//
//

#ifndef __ZombieSoccer__GameConfiguration__
#define __ZombieSoccer__GameConfiguration__

#include "PlayerControl.h"
#include "cocos2d.h"

#include <iostream>

using namespace cocos2d;

class GameConfiguration
{
public:
    GameConfiguration();
    ~GameConfiguration();
    
    CC_SYNTHESIZE(PlayerControl*, _lPlayerC, LeftPlayerC);
    CC_SYNTHESIZE(PlayerControl*, _rPlayerC, RightPlayerC);
    
    CC_SYNTHESIZE(int, _activePlayerID, ActivePlayerID);
    
    bool bothAreInitialized()
    {
        return _lPlayerC->getState().isInitialized
                &&
               _rPlayerC->getState().isInitialized;
    }
    
    void assignActivePlayer();
    
    
    void switchActivePlayer();
    
    
    bool touchAllowed()
    {
        if (_lPlayerC->getIsActive())
        {
            return _lPlayerC->whenActiveTouchAllowed();
        }
        if (_rPlayerC->getIsActive())
        {
            return _rPlayerC->whenActiveTouchAllowed();
        }
        
        return true;
    }
    
    bool noneIsShooting()
    {
        return !_lPlayerC->getState().hasShoot
                &&
               !_rPlayerC->getState().hasShoot;
    }
    
private:
    int randomFlip() 
	{
       return rand()%1;
    }
    
    int static const DEFAULT_ID = -1;
};

#endif /* defined(__ZombieSoccer__GameConfiguration__) */
