#include "pch.h"

#include "HelpLayer.h"

HelpLayer::HelpLayer()
{
    init();
}

HelpLayer::~HelpLayer()
{
    
}

bool HelpLayer::init()
{
	
    bool bRet = false;
	
	do 
	{
		CC_BREAK_IF(! OverlayBase::init());
		
		setTouchEnabled(true);
	
		CCSprite* bg = CCSprite::create("HelpLayer.png");
		
		bg->setAnchorPoint(ccp(0,0));
        bg->setPosition(ccp(0, 0));
		addChild(bg, -1);

        CCMenuItemSprite* qItem = createMenuItemSprite((char*)"btnClose.png", menu_selector(OverlayBase::onQuit));
        CCMenu* menu = CCMenu::create(qItem, NULL);
        
        menu->alignItemsVertically();  
        menu->setPosition(ccp(400,80));
		
        // add this to the layer
		this->addChild( menu, 1 );


		bRet = true;
	} while (0);

	return bRet;
}

