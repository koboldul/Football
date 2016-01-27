#include "pch.h"

#include "OverlayBase.h"
#include "cocos2d.h"

OverlayBase::OverlayBase()
{
    
}

OverlayBase::~OverlayBase()
{
    CCLOG("destroy ~OverlayBase");
    
}


bool OverlayBase::init()
{
    bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(0, 0, 0, 230)));
        
		bRet = true;
        
	} while (0);
    
	return bRet;
}

void OverlayBase::onQuit(CCObject *pSender)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(LAYER_QUIT, this);
}


CCMenuItemSprite* OverlayBase::createMenuItemSprite(char* name, SEL_MenuHandler func)
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(name);
    CCMenuItemSprite* item = CCMenuItemSprite::create(sprite, sprite, this, func);
    
    return item;
}

CCMenuItemSprite* OverlayBase::createMenuItemSprite(char* name, char* pressed, SEL_MenuHandler func)
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(name);
    CCSprite* spritep = CCSprite::createWithSpriteFrameName(pressed);
    
    CCMenuItemSprite* item = CCMenuItemSprite::create(sprite, spritep, this, func);
    
    return item;
}

