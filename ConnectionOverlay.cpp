//
//  ConnectionOverlay.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/8/13.
//
//

#include "ConnectionOverlay.h"


ConnectionOverlay::ConnectionOverlay()
{
    
}

ConnectionOverlay::~ConnectionOverlay()
{
    CCLOG("destroy ~GamePauseLayer");
}


bool ConnectionOverlay::init()
{
    bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(0, 0, 0, 200)));
		
        
        CCMenuItemSprite* qItem = createMenuItemSprite((char*)"btnArrow_Back128x128.png", menu_selector(ConnectionOverlay::onClose));
        
        CCMenu* menu = CCMenu::create(qItem, NULL);
        
        menu->alignItemsVertically();
        menu->setPosition(ccp(200,270));
		// add this to the layer
		this->addChild( menu, 1 );
        
        CCLabelTTF* message = CCLabelTTF::create("Finding an open game...", "Arial", 24);
        message->setPosition(ccp(400,100));
                             
        this->addChild(message);
        
		bRet = true;
	} while (0);
    
	return bRet;
}

void ConnectionOverlay::onClose(CCObject *pSender)
{
    getParent()->removeChild(this);
    
    CCScene* nextScene = MainMenuLayer::scene();
	CCDirector::sharedDirector()->replaceScene(nextScene);
    
}

ConnectionOverlay* ConnectionOverlay::create()
{
    ConnectionOverlay* pRet = new ConnectionOverlay();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
