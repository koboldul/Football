//
//  ConnectionOverlay.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/8/13.
//
//

#ifndef __ZombieSoccer__ConnectionOverlay__
#define __ZombieSoccer__ConnectionOverlay__

#include "OverlayBase.h"
#include "cocos2d.h"
#include "MainMenuLayer.h"

using namespace cocos2d;

class ConnectionOverlay : public OverlayBase
{
public:
    ConnectionOverlay();
    ~ConnectionOverlay();
    
    bool init();
    static ConnectionOverlay* create();
	
private:
    void onClose(CCObject* pSender);

};

#endif /* defined(__ZombieSoccer__ConnectionOverlay__) */
