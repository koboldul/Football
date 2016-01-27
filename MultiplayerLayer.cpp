//
//  MultiplayerLayer.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 8/13/13.
//
//

#include "MultiplayerLayer.h"

MultiplayerLayer::MultiplayerLayer()
{
    init();
}

MultiplayerLayer::~MultiplayerLayer()
{
    CCLOG("destroy ~MultiplayerLayer");
}


bool MultiplayerLayer::init()
{
    bool bRet = false;
	do
	{
		CC_BREAK_IF(! OverlayBase::init());
		
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		int leftSpace = 100;

        _opponentType = kHuman;
        
        CCMenuItemSprite* hItem = createMenuItemSprite((char*)"btnHotSeat128x128.png",
                                                       (char*)"btnHotSeatPressed.png",
                                                       menu_selector(MultiplayerLayer::onHotSeat));
        CCMenuItemSprite* onlineItem = createMenuItemSprite((char*)"btnOnlineGame128x128.png",
                                                       (char*)"btnOnlineGamePressed.png",
                                                       menu_selector(MultiplayerLayer::onOnlineGame));
        
        CCMenu* menu = CCMenu::create(hItem, onlineItem, NULL);
        menu->alignItemsVertically();
		menu->setPosition(ccp(leftSpace, size.height - 2 * leftSpace));
		addChild(menu, 1);
        
        
        if (!sgl_ConnectionManager.getRoomNumber().empty())
        {
            CCMenuItemSprite* onlineReconnectItem = createMenuItemSprite((char*)"btnOnlineGameReconnect.png",
                                                                (char*)"btnOnlineGamePressed.png",
                                                                menu_selector(MultiplayerLayer::onOnlineGame));
            
            menu->cocos2d::CCNode::addChild(onlineReconnectItem);
        }
        
		float menuLimit = hItem->getPosition().x + hItem->boundingBox().size.width + 20;

		CCLabelTTF* lH = new CCLabelTTF();
		lH->initWithString("Hot seat", S_F_TYPE.c_str(), 32);
		lH->setPosition(ccp(menuLimit, hItem->getPosition().y));
		lH->setAnchorPoint(ccp(0,0));
        hItem->addChild(lH);
        
        CCLabelTTF* lO = new CCLabelTTF();
        lO->initWithString("Random online game", S_F_TYPE.c_str(), 32);
        lO->setPosition(ccp(menuLimit, hItem->getPosition().y));
		lO->setAnchorPoint(ccp(0,0));
        onlineItem->addChild(lO);

        //Cancel - Go Menu
        CCMenuItemSprite* mRightArrow =
            sgl_SpriteFactory.createMenuItemSprite("btnArrow_Back128x128.png", "btnArrow_Back128x128Pressed.png", menu_selector(MultiplayerLayer::onQuit), this);
        CCMenu* menuReturn = CCMenu::create(mRightArrow, NULL);
        
        CCMenuItemSprite* mLeftArrow =
            sgl_SpriteFactory.createMenuItemSprite("btnArrow128x128.png", "btnArrow128x128Pressed.png", menu_selector(MultiplayerLayer::onConnect), this);
        
        CCMenu* mStartGame = CCMenu::create(mLeftArrow, NULL);
        
		menuReturn->setPosition(leftSpace, 150);
		mStartGame->setPosition(size.width - leftSpace - mLeftArrow->boundingBox().size.width * 0.5f, 150);
        
        addChild(mStartGame);
        addChild(menuReturn);
        
        
        _connectionStatus = CCLabelTTF::create("", S_F_TYPE.c_str(), 32);
		_connectionStatus->setPosition(ccp(size.width/2, 150));
        
        this->addChild(_connectionStatus);
        
		bRet = true;
	} while (0); 
    
	return bRet;
}


void MultiplayerLayer::onOnlineGame(CCObject* pSender)
{
    _opponentType = kHumanRemote;
    _connectionStatus->setString("Random online game selected");
}

void MultiplayerLayer::onOnlineReconnectGame(CCObject* pSender)
{
    _opponentType = kHumanRemoteReconnect;
    _connectionStatus->setString("Reconnect");
}

void MultiplayerLayer::onHotSeat(CCObject *pSender)
{
    _opponentType = kHuman;
    _connectionStatus->setString("Hot seat game selected");
}

void MultiplayerLayer::onConnect(CCObject *pSender)
{
    PlayerControl* rControl = new PlayerControl(1);
    PlayerControl* lControl;
    if (_opponentType == kHuman)
    {
        lControl = new PlayerControl(2);
    }
    else
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (_opponentType == kHumanRemoteReconnect)
        {
            sgl_ConnectionManager.setRoomNumber("");
        }
		lControl = new RemoteControl(2);
#else
		lControl = new PlayerControl(2);
#endif
    }
    
    lControl->setPlayerName(CCUserDefault::sharedUserDefault()->getStringForKey(USER_DATA_NAME));
    
    rControl->init();
    lControl->init();
    
	CCScene* nextScene = CCTransitionFadeBL::create(0.9f, StadionLayer::scene(lControl, rControl));
	CCDirector::sharedDirector()->replaceScene(nextScene);
}