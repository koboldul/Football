#include "pch.h"

#include "StadionLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace SoccerGlobal;

const int _pauseLayerTag = 333;
const int _gameWonSpriteTag = 444;
const int _gameLostSpriteTag = 555;

//Constructors
StadionLayer::StadionLayer(PlayerControl* lControl, PlayerControl* rControl)
{
    _running = false;
    _canShoot = true;
    _shooting = false;
    _gameState = kGamePlay;
    
	_screenSize = CCDirector::sharedDirector()->getWinSize();
	
    setRightControl(rControl);
    setLeftControl(lControl);
    
    getRightControl()->setIsActive(true);
    
	initWorldData();
    init();
}

StadionLayer::~StadionLayer()
{
	if (_debugDraw != NULL)
	{
		delete _debugDraw;
		_debugDraw = NULL;
	}

    delete _world;
    _world = NULL;  

    delete _ballSynchronizer;
    _ballSynchronizer = NULL;
  
    delete _rightPlayer;
    delete _leftPlayer;
    _rightPlayer = NULL;
    _leftPlayer = NULL;
    
    delete _rightControl;
    delete _leftControl;
    _rightControl = NULL;
    _leftControl = NULL;
    
    delete _gameConfig;
    _gameConfig = NULL;
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, RESOLVE_SHOOT);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LAYER_QUIT);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GAME_QUIT);
    
    CCLOG("destroy ~Stadion layer");
}

//Public:
//@override
bool StadionLayer::init()
{
	bool bRet = false;
	_shooting = false;
    syncTimer = 0;
    
	do 
	{
		if ( !CCLayer::init() || !initVisuals())
		{
			break;
		}
		setTouchEnabled(true);
	
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StadionLayer::onResolveShooting), RESOLVE_SHOOT, NULL);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StadionLayer::onGameQuit), GAME_QUIT, NULL);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StadionLayer::onLayerClose), LAYER_QUIT, NULL);
                
		scheduleUpdate();
        
        _timer = sgl_SpriteFactory.createTimerLabel(kRight);
        
		addChild(_timer, 4);
//        this->schedule(schedule_selector(StadionLayer::ticktock), 1.5f);
        
        _gameConfig = new GameConfiguration();
        _gameConfig->setLeftPlayerC(getLeftControl());
        _gameConfig->setRightPlayerC(getRightControl());
        getLeftControl()->setSide(kLeft);
        getRightControl()->setSide(kRight);
      
		bRet = true;
	} while (0);

	return bRet;
}

bool StadionLayer::initVisuals()
{
    _goalIsVisibleCounter = 0;
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(stadionListName);
    CCSpriteBatchNode *node = CCSpriteBatchNode::create(stadionImgName);
    addChild(node);
    
    addChild(createStadionMenu(), 1 );
    
    b2Vec2 gravity;
    gravity.Set(0.0f, -0.0f);
    _world = new SoccerWorld(this, gravity);
    if (!_world->initWorld())
    {
        return false;
    }
    
	_debugDraw = new GLESDebugDraw(PTM_RATIO);
	_world->SetDebugDraw(_debugDraw);

	uint32 flags = 0;
	  flags += b2Draw::e_shapeBit;
	  flags += b2Draw::e_jointBit;
	  flags += b2Draw::e_aabbBit;
	  flags += b2Draw::e_pairBit;
	  flags += b2Draw::e_centerOfMassBit;
	  _debugDraw->SetFlags(flags);

    _lineContainer = LineContainer::create();
    addChild(_lineContainer);
    
	addChild(sgl_SpriteFactory.createBackground(), -1);
    
    _attackerArrow = sgl_SpriteFactory.createArrow();
    _attackerArrow->setRotation(180);
    addChild(_attackerArrow, 4);
    
    _goalAd = sgl_SpriteFactory.createGoal();
    _goalAd->setVisible(false);
    addChild(_goalAd, 10);
    
    addChild(sgl_SpriteFactory.createTribuna(0, 0));
    addChild(sgl_SpriteFactory.createTribuna(1, 0));
    addChild(sgl_SpriteFactory.createTribuna(2, 0));

    addChild(sgl_SpriteFactory.createTribuna(0, 1));
    addChild(sgl_SpriteFactory.createTribuna(1, 1));
    addChild(sgl_SpriteFactory.createTribuna(2, 1));
    
    setRigthPlayer(new Player());
    setLeftPlayer(new Player());
    
    getRigthPlayer()->init(this, kRight);
    getLeftPlayer()->init(this, kLeft);
    
    _terrain = Terrain::createWithSizeAndWorld(_screenSize, this,
                                               getRigthPlayer()->getGate()->boundingBox().size);
    
    CCPoint center = _terrain->getCenter();
    
    _ball = Ball::create(this, center);
    
    _ballSynchronizer = new BallSynchronizer(_ball);
    
    this->addChild(_terrain, 0);
    this->addChild(_ball, 2);
    
    _playerLeftScoreLabel = new CCLabelTTF();
    _playerRightScoreLabel = new CCLabelTTF();
    
    initScoreLabel(_playerLeftScoreLabel, kRight);
    initScoreLabel(_playerRightScoreLabel, kLeft);

    TerrainMap::getInstance().setRightPlayer(getRigthPlayer());
    TerrainMap::getInstance().setLeftPlayer(getLeftPlayer());
    
	//CCLayerGradient* layer =  new CCLayerGradient();
	//layer->setStartOpacity(255);
	//layer->setStartOpacity(0);
	//layer->setStartColor(ccc3(0,0,0));
	////layer->setVector(ccp(0,));

	//addChild(layer, 1);
 //   layer->release();

    return true;
}

//override
void StadionLayer::draw()
{
	//
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    CCLayer::draw();

//#if (CC_ENABLE_BOX2D_INTEGRATION)
ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

kmGLPushMatrix();

_world->DrawDebugData();

kmGLPopMatrix();
//#endif
}

void StadionLayer::update(float dt)
{
    ////////////////
    // GOAL is showing
    if (kGamePaused == _gameState && _goalAd->isVisible())
    {
        _goalIsVisibleCounter++;
        
        if (120 == _goalIsVisibleCounter)
        {
            _goalAd->setVisible(false);
            _goalIsVisibleCounter = 0;
            _gameState = kGamePlay;
        }
    }
    
    if (!(_gameState == kGameOver || _gameState == kGamePaused))
    {
        _world->Step(dt, 10, 10);
    
        markActivePlayer();
    
        if (_ball->getBody()->GetLinearVelocity().Length() <= 0)
        {
            _ballSynchronizer->update(dt);
            
            if (_shooting && getGameConfig()->noneIsShooting())
            {
                _shooting = false;
                
                getRightControl()->update(_ball->getPosition(), b2Vec2(0,0));
                getLeftControl()->update(_ball->getPosition(), b2Vec2(0,0));
    
                syncTimer = 0;
                
                _ballSynchronizer->setSynchronizationPoint(CCPointZero);
                
                _gameConfig->switchActivePlayer();
            }
        }
        else
        {
            _ballSynchronizer->update(dt);
            
            syncTimer += dt;
            
            if (syncTimer > 0.3)
            {
                getRightControl()->update(_ball->getPosition(), _ball->getBody()->GetLinearVelocity());
                getLeftControl()->update(_ball->getPosition(), _ball->getBody()->GetLinearVelocity());
            }
        }
		
        checkForGoal(getRigthPlayer(), getLeftPlayer(), _playerLeftScoreLabel);
        checkForGoal(getLeftPlayer(), getRigthPlayer(), _playerRightScoreLabel);
    }
    
    
    CCObject* overlay = getChildByTag(PAUSE_LAYER_TAG);
    
    if(!getGameConfig()->bothAreInitialized() && overlay == NULL)
    {
		_overlay = ConnectionOverlay::create();
        _overlay->setTag(PAUSE_LAYER_TAG);
        addChild(_overlay, 10);
        _gameState = kGamePaused;
    }
    else if(getGameConfig()->bothAreInitialized() && overlay != NULL)
    {
        _gameState = kGamePlay;
        removeChild((CCLayer*)overlay);
        getGameConfig()->assignActivePlayer();
    }
}

//Essential for displaying anything.
CCScene* StadionLayer::scene(PlayerControl* lControl, PlayerControl* rControl)
{
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    StadionLayer* layer = new StadionLayer(lControl, rControl);
        
    scene->addChild(layer, 1);
    //scene->addChild(hudLayer, 5);
    
    layer->release();
    //hudLayer->release();
    
    
    
    return scene;
}

/*
 * EVENTS
 */
void StadionLayer::ccTouchesBegan(CCSet* touches, CCEvent* event)
{

    if (_gameState == kGameOver
        ||
        _gameState == kGamePaused
        ||
        !getGameConfig()->touchAllowed()
        
        ) {
        
        return;
    }
	
    //
    CCTouch *touch = (CCTouch *)touches->anyObject();
    //
    if (touch && !_shooting) {
         CCPoint tap = touch->getLocation();
        
        //if (TerrainMap::getInstance().isPointInPlayField(tap))
        {
            _shootStartPoint = tap;
		
            float length = ccpDistance(_shootStartPoint, _terrain->getCenter());
            if (length < 20)
            {
                _ball->reset();
                _ball->setPosition(_terrain->getCenter());
                return;
            }
		
            _lineContainer->setDrawing(true);
            _lineContainer->setBallPoint(_shootStartPoint);
        }
    }
}

void StadionLayer::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    
    if (_gameState == kGameOver || _gameState == kGamePaused
        ||
        !getGameConfig()->touchAllowed())
    {
        return;
    }
    
    CCTouch *touch = (CCTouch *)touches->anyObject();    
	
    if(touch && !_shooting)
	{
       CCPoint p0 = touch->getLocation();
       _lineContainer->setCuePoint(p0);
   }
}

void StadionLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    
    if (_gameState == kGameOver || _gameState == kGamePaused
        ||
        !getGameConfig()->touchAllowed())
    {
        return;
    }
    
    CCTouch *touch = (CCTouch *)touches->anyObject();    

	if(touch && !_shooting)
	{
        ObserverData* data = new ObserverData();
        data->setInitialPosition(_shootStartPoint);
        data->setFinalPosition(touch->getLocation());
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(START_SHOOT, data);
	}
}

bool StadionLayer::isPlayerTouched(CCTouch *touch)
{
    return true;
}

void StadionLayer::initWorldData()
{
    float height = _screenSize.height - 2 * XOffset - 2 * MARGIN_TICKNESS;
	
	float f = height / SoccerGlobal::TerrainAspectRatio;
	float offset =  0.15f * _screenSize.width;
    if (f > _screenSize.width - offset)
    {
        f = _screenSize.width - offset;
        height = f * SoccerGlobal::TerrainAspectRatio;
    }
    
    setTerrainHeight(height);
	setTerrainWidth(f);
	
	sgl_Settings.setXOffset((_screenSize.width - getTerrainWidth()) * 0.5f);
	sgl_Settings.setYOffset((_screenSize.height - getTerrainHeight()) * 0.5f);
    
    sgl_Settings.setTerrainWidth(getTerrainWidth());
	sgl_Settings.setTerrainHeight(getTerrainHeight());
}	

void StadionLayer::updateScoreLabel(CCLabelTTF * label, int score)
{
    char lt[100] = {0};
	sprintf(lt, "%i", score);
    
	label->setString(lt);
}

void StadionLayer::initScoreLabel(CCLabelTTF * label, TerrainPosition pos)
{
	int score = pos == kRight ? _rightPlayer->getScore() : _leftPlayer->getScore();
    int xOff = pos == kRight ? _screenSize.width - 10 : 10;
	CCPoint anchor = pos == kRight ? ccp(1,1) : ccp(0,1);

	ccColor3B color = pos == kRight ? ccc3(255,255, 255) : ccc3(255, 255, 255);
    
    char lS[100] = {0};
    sprintf(lS, "%i", score);
    
	label->initWithString(lS, S_F_TYPE.c_str(), _screenSize.width * 0.05f);
	label->setAnchorPoint(anchor);
	label->setPosition(ccp(xOff, _screenSize.height - 10));
	
	label->setColor(color);
		
	this->addChild(label, 10);

	if (pos == kRight)
	{
		_playerRightName = CCLabelTTF::create(getRightControl()->getPlayerName().c_str(), S_F_TYPE.c_str(), 40);
		_playerRightName->setPosition(ccp(_screenSize.width - 10 - _playerRightName->boundingBox().size.height , _screenSize.height-70));
		_playerRightName->setAnchorPoint(ccp(1,1));
		_playerRightName->setRotation(270);
		addChild(_playerRightName);
	}
	else
	{
		_playerLeftName = CCLabelTTF::create(getLeftControl()->getPlayerName().c_str(), S_F_TYPE.c_str(), 40);
		_playerLeftName->setPosition(ccp(10 + _playerLeftName->boundingBox().size.height , _screenSize.height-70));
		_playerLeftName->setAnchorPoint(ccp(0,1));
		_playerLeftName->setRotation(90);
		addChild(_playerLeftName);
	}
}

void StadionLayer::formatTime(char* time, int absoluteTime)
{
    int seconds = absoluteTime % 60;
    int minutes = (absoluteTime / 60) % 60;
    
    if (seconds < 10)
    {
        sprintf(time, "%i:0%i", minutes, seconds);
    }
    if (seconds >= 10)
    {
        sprintf(time, "%i:%i", minutes, seconds);
    }

}

void StadionLayer::ticktock()
{
    int rtime = getRightControl()->getTime();
    char rt[100] = {0};
    formatTime(rt, rtime);
    
    _timer->setString(rt);
}

CCMenu* StadionLayer::createStadionMenu()
{
    CCMenuItemSprite* pause = sgl_SpriteFactory.createMenuItemSprite("btnPause.png", "btnPausePressed.png",menu_selector(StadionLayer::onPause), this);
    CCMenuItemSprite* help = sgl_SpriteFactory.createMenuItemSprite("btnHelp.png", "btnHelp.png",menu_selector(StadionLayer::onHelp), this);
    
    CCMenu* menu = CCMenu::create(pause, help, NULL);
    
    menu->alignItemsVerticallyWithPadding(2);
    menu->setPosition(ccp(32,60));
    
    return menu;
}

void StadionLayer::onPause(CCObject* pSender)
{
	CCLayer* layer =  new GamePauseLayer();
    addChild(layer, 10);
    
    _gameState = kGamePaused;
    
    layer->release();
}

void StadionLayer::onHelp(CCObject* pSender)
{
	CCLayer* layer =  new HelpLayer();
    addChild(layer, 10);
    
    _gameState = kGamePaused;
    
    layer->release();
}

void StadionLayer::onLayerClose(CCObject* layer)
{
    _gameState = kGamePlay;
    removeChild((CCLayer*)layer);
}

void StadionLayer::onGameQuit(CCObject* layer)
{
    getLeftControl()->gameQuit();
    getRightControl()->gameQuit();
    
    removeChild((CCLayer*)layer);
    
    CCScene* nextScene = CCTransitionFadeBL::create(0.9f, MainMenuLayer::scene());
	CCDirector::sharedDirector()->replaceScene(nextScene);
}

void StadionLayer::onResolveShooting(CCObject* param)
{
	b2Vec2 velocity = dynamic_cast<ObserverData *>(param)->getSpeed();
	_shooting = true;
    
	SimpleAudioEngine::sharedEngine()->playEffect(BALL_SOUND);
    
    _ball->startShooting(velocity);
	
    
	_shootStartPoint = CCPoint(0,0);
    
	_lineContainer->setDrawing(false);
	_lineContainer->setCuePoint(ccp(0,0));
	_lineContainer->setBallPoint(ccp(0,0));
}

void StadionLayer::checkForGoal(Player* player, Player* opponent, CCLabelTTF* label)
{
    CCPoint ballCenter = ccp(_ball->getPosition().x, _ball->getPosition().y);
    
    if (player->getGate()->getHoldsBall())
    {
        bool isGoal = false;
        isGoal = ballCenter.x > (player->getGate()->getPosition().x -
                                 player->getGate()->getContentSize().width * 0.5f);
        
        if (isGoal)
        {
            _gameState = kGamePaused;
            
			int score = opponent->getScore();
            score++;
            opponent->setScore(score);
            
            updateScoreLabel(label, score);
            player->getGate()->setHoldsBall(false);
            
			int goals = sgl_Settings.getOptions().Score;

			if (opponent->getScore() >= goals)
			{
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CROWD_SOUND, false);
				_goalAd->setVisible(true);
			}
			else
			{
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CROWD_SOUND, false);
				CCLabelTTF* won = CCLabelTTF::create(getRightControl()->getPlayerName().c_str(), S_F_TYPE.c_str(), 100); 
				won->setTag(_gameWonSpriteTag);
				addChild(won);

				_gameState = kGameOver;
			}
            
            _ball->reset();
        }
    }	
}

void StadionLayer::markActivePlayer()
{
	bool rightIsActive = _rightControl->getIsActive();
	
	float rotation = rightIsActive ? 180 : 0;
    _attackerArrow->setRotation(rotation);

	CCLabelTTF* labelActive = rightIsActive ? _playerRightName : _playerLeftName;
	CCLabelTTF* labelInActive = !rightIsActive ? _playerRightName : _playerLeftName;

	labelActive->setOpacity(255);
	labelInActive->setOpacity(175);
}