#ifndef __STADIONLAYER_SCENE_H__
#define __STADIONLAYER_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "SoccerWorld.h"
#include "SoccerGlobal.h"
#include "LineContainer.h"
#include "Terrain.h"
#include "Player.h"
#include "PlayerControl.h"
#include "GamePauseLayer.h"
#include "ObserverData.h"
#include "SpriteFactory.h"
#include "HelpLayer.h"
#include "GameConfiguration.h"
#include "TerrainMap.h"
#include "BallSynchronizer.h"
#include "ConnectionOverlay.h"

#include "GLES-Render.h"

class LineContainer;
class Terrain;
class Player;
class PlayerControl;


using namespace SoccerGlobal;

enum {
    kGameIntro,
    kGamePlay,
    kGameOver,
    kGamePaused,
    kGameGoalPaused
};

class StadionLayer : public CCLayer {
public:
    ~StadionLayer();
    StadionLayer(PlayerControl* lControl, PlayerControl* rControl);
    
	CC_SYNTHESIZE(SoccerWorld *, _world, World);
	CC_SYNTHESIZE(float, _stadionW, TerrainWidth);
	CC_SYNTHESIZE(float, _stadionH, TerrainHeight);

	CC_SYNTHESIZE(Player*, _rightPlayer, RigthPlayer);
	CC_SYNTHESIZE(Player*, _leftPlayer, LeftPlayer);
    CC_SYNTHESIZE(GameConfiguration*, _gameConfig, GameConfig);

	CC_SYNTHESIZE(PlayerControl*, _rightControl, RightControl);
	CC_SYNTHESIZE(PlayerControl*, _leftControl, LeftControl);

    // returns a Scene that contains the HelloWorld class as the only child
	static cocos2d::CCScene* scene(PlayerControl* lControl, PlayerControl* rControl);
    
	virtual bool init();
    virtual void draw();
    void update(float dt);
    void ticktock(void);
    
    float syncTimer;
    
	void initScoreLabel(CCLabelTTF * label, TerrainPosition pos);
	
	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
 
	void onResolveShooting(CCObject* param);
	
    bool isPlayerTouched(CCTouch* touch);

private:
    
	void initWorldData();
    bool initVisuals();
    
    CCMenu* createStadionMenu();
    void onPause(CCObject* pSender);
    void onHelp(CCObject* pSender);
    void onLayerClose(CCObject* layer);
    void checkForGoal(Player* player, Player* opponent, CCLabelTTF* label);
    void onGameQuit(CCObject* layer);
    
    void formatTime(char* time, int absoluteTime);
    void updateScoreLabel(CCLabelTTF * label, int score);
	void markActivePlayer();
    
    CCTouch * _touch;
	Ball * _ball;
    CCSprite* _attackerArrow;
    CCSprite* _goalAd;
	
    Terrain * _terrain;
	LineContainer * _lineContainer;
    
    CCSpriteBatchNode * _gameBatchNode;
    CCSprite * _intro;
    CCSprite * _gameOver;
    CCLabelTTF * _timer;
    BallSynchronizer * _ballSynchronizer;
    
    CCPoint _shootStartPoint;
    CCSize _screenSize;
    
    bool _running ;
       
    int _gameState ;
    int _goalIsVisibleCounter;
    
	bool _canShoot ;
	bool _shooting  ;

    CCLayer* _overlay;
    
	CCLabelTTF * _playerLeftScoreLabel;
    CCLabelTTF * _playerRightScoreLabel;

	CCLabelTTF * _playerLeftName;
    CCLabelTTF * _playerRightName;
    
    CCPoint _newBallPos;

	GLESDebugDraw* _debugDraw;
    
    int static const PAUSE_LAYER_TAG = 33333;
};

#endif  // __STADIONLAYER_SCENE_H__
