#ifndef __MainMenuLayer__
#define __MainMenuLayer__

#define REMOTE_CONTROL_INCLUDE "RemoteControl.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
//#include REMOTE_CONTROL_INCLUDE
#endif

#include "cocos2d.h"
#include "StadionLayer.h"
#include "SoccerGlobal.h"
#include "MultiplayerLayer.h"
#include "HelpLayer.h"
#include "AIControl.h"
#include "OptionsLayer.h"

using namespace cocos2d;
using namespace SoccerGlobal;

class MainMenuLayer : public CCLayer 
{
public:
	virtual bool init();
	static CCScene* scene();
    
    MainMenuLayer();
    ~MainMenuLayer();

	void onNewGame(CCObject* pSender);
	void onQuit(CCObject* pSender);
	void onMultiplayer(CCObject* pSender);
	void onHelp(CCObject* pSender);
    void onSettingsChoose(CCObject* pSender);

private:
    CCMenuItemSprite* createMenuIetSprite(char* name, SEL_MenuHandler func);
    void onLayerClose(CCObject* layer);
	void enableMenus(bool willEnable);

	CCLabelTTF* _lblWelcome;

	CCMenu* _menuSet;
	CCMenu* _menuGame;
};

#endif //__MainMenuLayer__