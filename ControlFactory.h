//
//  ControlFactory.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/2/13.
//
//

#ifndef __ZombieSoccer__ControlFactory__
#define __ZombieSoccer__ControlFactory__

#include "cocos2d.h"
#include "SoccerGlobal.h"
#include "b2Sprite.h"
#include <cocos-ext.h>

using namespace SoccerGlobal;
USING_NS_CC;
USING_NS_CC_EXT;

#define sgl_ControlFactory ControlFactory::getInstance()

class ControlFactory
{
public:
    static ControlFactory& getInstance()
    {
        static ControlFactory instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
	CCLabelTTF* createTTFLabel(std::string text, CCPoint pos);
	CCLabelBMFont* createBMFontLabel(std::string text, CCPoint pos);

private:
    ControlFactory() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    ControlFactory(ControlFactory const&); // Don't Implement
    void operator=(ControlFactory const&); // Don't implement
};



#endif /* defined(__ZombieSoccer__ControlFactory__) */
