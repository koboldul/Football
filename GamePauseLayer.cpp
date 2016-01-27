//
//  GamePauseLayer.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 8/19/13.
//
//

#include "GamePauseLayer.h"

GamePauseLayer::GamePauseLayer()
{
    init();
}

GamePauseLayer::~GamePauseLayer()
{
    CCLOG("destroy ~GamePauseLayer");
}

bool GamePauseLayer::init()
{
    bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(0, 0, 0, 200)));
		
        
        CCMenuItemSprite* qItem = createMenuItemSprite((char*)"btnInfo.png", menu_selector(OverlayBase::onQuit));
        CCMenuItemSprite* hItem = createMenuItemSprite((char*)"btnClose.png", menu_selector(GamePauseLayer::onClose));
        
        CCMenu* menu = CCMenu::create(hItem, qItem, NULL);
        
        menu->alignItemsVertically();
        menu->setPosition(ccp(200,270));
		// add this to the layer
		this->addChild( menu, 1 );
        
		bRet = true;
	} while (0);
    
	return bRet;
}

void GamePauseLayer::onClose(CCObject *pSender)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GAME_QUIT, this);
}

