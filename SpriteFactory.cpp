//
//  SpriteFactory.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/2/13.
//
//

#include "SpriteFactory.h"

CCSprite* SpriteFactory::createArrow()
{
    CCSprite* arrow = CCSprite::createWithSpriteFrameName("Arrow.png");
    Settings::getInstance().scaleSprite(arrow, 0.3f);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    arrow->setPosition(ccp(size.width/2,
                           Settings::getInstance().getYOffset() + Settings::getInstance().getTerrainHeight() - 30));
    arrow->setOpacity(175);

    return arrow;
}

CCSprite* SpriteFactory::createGoal()
{
    CCSprite* goalAd = CCSprite::createWithSpriteFrameName("GOAL.png");
    //Settings::getInstance().scaleSprite(arrow, 0.3f);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    goalAd->setPosition(ccp(size.width/2, size.height/2));
    
    
    return goalAd;
}

CCSprite* SpriteFactory::createBackground()
{
    CCSprite* bg = CCSprite::create("bg.png");
    
    bg->setAnchorPoint(ccp(0,0));
    bg->setPosition(ccp(0,0));
    
    return bg;
}

CCSprite* SpriteFactory::createTribuna(int order, int pos)
{
    
    CCSprite* tribuna = CCSprite::createWithSpriteFrameName("tribuna.png");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (pos == 0)
    {
        tribuna->setScaleY(0.30f);
        tribuna->setScaleX(0.30f);
        //tribuna->setAnchorPoint(ccp(0,0));
    
        tribuna->setPosition(ccp(order * tribuna->boundingBox().size.width + 200, 30));
        tribuna->setRotation(180);
    }
    
    if (pos == 1)
    {
        tribuna->setScaleY(0.30f);
        tribuna->setScaleX(0.30f);
        
        tribuna->setPosition(ccp(order * tribuna->boundingBox().size.width + 200, size.height - 30));
    }
    
    return tribuna;
}

CCLabelTTF* SpriteFactory::createTimerLabel(int side)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF* timer = CCLabelTTF::create("0", "Consola", size.height * 0.03f);
    
    CCPoint p = ccp(size.width * 0.5f, size.height);
    
	timer->setAnchorPoint(ccp(0.5f, 1));
    timer->setPosition(p);
    timer->setVisible(true);
    
    return timer;
}

CCMenuItemSprite* SpriteFactory::createMenuItemSprite(std::string name, std::string nameSelected, SEL_MenuHandler func, CCObject* target)
{
	CCSprite* sprite = CCSprite::createWithSpriteFrameName(name.c_str());
	CCSprite* spriteSelected = CCSprite::createWithSpriteFrameName(nameSelected.c_str());
    
    CCMenuItemSprite* item = CCMenuItemSprite::create(sprite, spriteSelected, target, func);
    
    return item;
}


