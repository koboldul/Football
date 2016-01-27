#ifndef __OverlayBase__
#define __OverlayBase__

#include "cocos2d.h"
#include "SoccerGlobal.h"

using namespace cocos2d;
using namespace SoccerGlobal;

class OverlayBase : public CCLayerColor
{
public:
	OverlayBase();
    ~OverlayBase();
    
    virtual bool init();
    
    
	
    void onQuit(CCObject* pSender);
    
protected:
    CCMenuItemSprite* createMenuItemSprite(char* name, char* pressed, SEL_MenuHandler func);
    CCMenuItemSprite* createMenuItemSprite(char* name, SEL_MenuHandler func);
};

#endif //__OverlayBase__