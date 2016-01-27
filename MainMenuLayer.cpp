#include "pch.h"

#include "MainMenuLayer.h"

using namespace CocosDenshion;

std::string WELCOME = "Welcome: ";

MainMenuLayer::MainMenuLayer()
{
    init();
}

MainMenuLayer::~MainMenuLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LAYER_QUIT);
}

bool MainMenuLayer::init()
{
	bool bRet = false;
	
	do 
	{
		if ( !CCLayer::init() )
		{
			break;
		}
		setTouchEnabled(true);
	           
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MainMenuLayer::onLayerClose), LAYER_QUIT, NULL);
        
        //Background
		CCSprite* bg = CCSprite::create("MainMenuBg.png");
		bg->setAnchorPoint(ccp(0,0));
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		bg->setScaleX(size.width / (bg->getContentSize().width + 100));
		bg->setScaleY(size.height / (bg->getContentSize().height + 100));

		bg->setPosition(ccp(0, 0));
		this->addChild(bg, -1);
        
        //Rest of items
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(mainMenuListName);
        CCSpriteBatchNode *node = CCSpriteBatchNode::create(mainMenuImgName);
        this->addChild(node);
        

		CCSprite* title = CCSprite::createWithSpriteFrameName("ZSoccerTitle.png");

		bg->setScaleX(size.width / bg->getContentSize().width);
		bg->setScaleY(size.height / bg->getContentSize().height);

		title->setPosition(ccp(size.width/2, size.height - 30 - title->boundingBox().size.height/2));
		this->addChild(title, 0);

		CCLabelTTF* cop = sgl_ControlFactory.createTTFLabel("copyright: Bogdan Costea 2014", ccp(size.width/2, 20));
		cop->setFontSize(15);
		this->addChild(cop, 0);
        
		/***********************************
		* Menu games layers
		***********************************/
        CCMenuItemSprite* training = sgl_SpriteFactory.createMenuItemSprite("NewGame.png", "NewGamePressed.png",menu_selector(MainMenuLayer::onNewGame), this);
		CCMenuItemSprite* multi = sgl_SpriteFactory.createMenuItemSprite("Options.png", "MultiplayerPressed.png",menu_selector(MainMenuLayer::onMultiplayer), this);
        
        _menuGame = CCMenu::create(training, multi, NULL);

		_menuGame->alignItemsVertically();
		_menuGame->setPosition(ccp(180,370));
		// add this to the layer
		this->addChild( _menuGame, 1 );
        
		/***********************************
		* Menu heleper layers
		***********************************/
        CCMenuItemSprite* settings = sgl_SpriteFactory.createMenuItemSprite("btnSettings.png", "btnSettingsPressed.png",menu_selector(MainMenuLayer::onSettingsChoose), this);
        CCMenuItemSprite* info = sgl_SpriteFactory.createMenuItemSprite("btnInfo.png", "btnInfoPressed.png",menu_selector(MainMenuLayer::onHelp), this);
        CCMenuItemSprite* help = sgl_SpriteFactory.createMenuItemSprite("btnHelp.png", "btnHelpPressed.png",menu_selector(MainMenuLayer::onHelp), this);
        
        CCMenuItemSprite* quit = sgl_SpriteFactory.createMenuItemSprite("btnClose.png", "btnClosePressed.png",menu_selector(MainMenuLayer::onQuit), this);
        
        _menuSet = CCMenu::create(settings, info, help, quit, NULL);
        
        _menuSet->alignItemsHorizontally();
        _menuSet->setPosition(ccp(size.width - 3 * 50 ,40));
		
		addChild(_menuSet, 1);

        std::string userName = CCUserDefault::sharedUserDefault()->getStringForKey(USER_DATA_NAME);
        
		_lblWelcome = sgl_ControlFactory.createTTFLabel((WELCOME + userName).c_str(), 
			ccp(title->getPositionX() + 10, title->getPositionY() - title->boundingBox().size.height * 0.5f - 2));
        _lblWelcome->setFontSize(25);
        _lblWelcome->setAnchorPoint(ccp(0,1));

		addChild(_lblWelcome, 0);
        
		bRet = true;
	} while (0);

	return bRet;
}

CCScene* MainMenuLayer::scene()
{
	CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new MainMenuLayer();
    scene->addChild(layer, 1);
    

    layer->release();
    
    return scene;
}

void MainMenuLayer::onNewGame(CCObject* pSender)
{
	PlayerControl* rControl = new PlayerControl(1);
    AIControl* lControl = new AIControl(2);

    rControl->init();
    lControl->init();
        
	CCScene* nextScene = CCTransitionFadeBL::create(0.9f, StadionLayer::scene(lControl, rControl));
	CCDirector::sharedDirector()->replaceScene(nextScene);
}

void MainMenuLayer::onHelp(CCObject* pSender)
{
	enableMenus(false);
	CCLayer* layer =  new HelpLayer();
    addChild(layer, 1);
    
    layer->release();
}

void MainMenuLayer::onSettingsChoose(CCObject* pSender)
{
	enableMenus(false);
	CCLayer* layer =  new OptionsLayer();
    addChild(layer, 1);
    
    layer->release();
}

void MainMenuLayer::onMultiplayer(CCObject* pSender)
{
	enableMenus(false);

	CCLayer* layer =  new MultiplayerLayer();
    addChild(layer, 1);
    
    layer->release();
}

void MainMenuLayer::onQuit(CCObject* pSender)
{
 // Exit game
 CCDirector::sharedDirector()->end();
    exit(0);
}

void MainMenuLayer::onLayerClose(CCObject* layer)
{
	std::string userName = CCUserDefault::sharedUserDefault()->getStringForKey(USER_DATA_NAME);
	_lblWelcome->setString((WELCOME + userName).c_str());

	enableMenus(true);	
    removeChild((CCLayer*)layer);
}

void MainMenuLayer::enableMenus(bool willEnable)
{
	_menuSet->setEnabled(willEnable);
	_menuGame->setEnabled(willEnable);
}

