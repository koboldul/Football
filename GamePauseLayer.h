//
//  GamePauseLayer.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 8/19/13.
//
//

#ifndef __ZombieSoccer__GamePauseLayer__
#define __ZombieSoccer__GamePauseLayer__

#include "MainMenuLayer.h"
#include "OverlayBase.h"
#include "cocos2d.h"

using namespace cocos2d;

class GamePauseLayer : public OverlayBase
{
public:
    GamePauseLayer();
    ~GamePauseLayer();
    
    bool init();
	
private:
    void onClose(CCObject* pSender);
	
	
};

#endif /* defined(__ZombieSoccer__GamePauseLayer__) */
