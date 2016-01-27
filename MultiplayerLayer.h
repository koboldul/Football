//
//  MultiplayerLayer.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 8/13/13.
//
//

#ifndef __ZombieSoccer__MultiplayerLayer__
#define __ZombieSoccer__MultiplayerLayer__


#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "RemoteControl.h"
#endif

#include "cocos2d.h"
#include "PlayerControl.h"
#include "OverlayBase.h"
#include "StadionLayer.h"
#include "ConectionManager.h"

using namespace cocos2d;

class MultiplayerLayer : public OverlayBase, public CCTextFieldDelegate
{
public:
    MultiplayerLayer();
    ~MultiplayerLayer();
    
    bool init();
	

private:
    
    CCLabelTTF* _connectionStatus;
    
	void onConnect(CCObject* pSender);
    
    
	void onHotSeat(CCObject* pSender);
    void onOnlineGame(CCObject* pSender);
    void onOnlineReconnectGame(CCObject* pSender);
    
    OpponentType _opponentType;
};
#endif /* defined(__ZombieSoccer__MultiplayerLayer__) */
