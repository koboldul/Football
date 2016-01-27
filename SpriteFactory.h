//
//  SpriteFactory.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/2/13.
//
//

#ifndef __ZombieSoccer__SpriteFactory__
#define __ZombieSoccer__SpriteFactory__

#include "cocos2d.h"
#include "SoccerGlobal.h"
#include "b2Sprite.h"

using namespace SoccerGlobal;
USING_NS_CC;

#define sgl_SpriteFactory SpriteFactory::getInstance()

class SpriteFactory
{
public:
    static SpriteFactory& getInstance()
    {
        static SpriteFactory instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    CCSprite* createArrow();
    CCSprite* createGoal();
    CCSprite* createBackground();
    CCSprite* createTribuna(int order, int pos);
    CCLabelTTF* createTimerLabel(int side);
    CCMenuItemSprite* createMenuItemSprite(std::string name, std::string nameSelected, SEL_MenuHandler func, CCObject* target);
    
private:
    SpriteFactory() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    SpriteFactory(SpriteFactory const&); // Don't Implement
    void operator=(SpriteFactory const&); // Don't implement
};



#endif /* defined(__ZombieSoccer__SpriteFactory__) */
