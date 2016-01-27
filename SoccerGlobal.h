
#ifndef __Soccer_Global__
#define __Soccer_Global__

#include "cocos2d.h"
#include "GameOptions.h"

#define sgl_Settings SoccerGlobal::Settings::getInstance()

using namespace cocos2d;

#define PTM_RATIO 32
#define SHOT_POWER 12.0
#define BALL_RADIUS 13.0
#define PLAYER_RADIUS 6.0

//OBSERVER ACTIONS

#define RESOLVE_SHOOT "resolveShoot"
#define RESOLVE_REMOTE "resolveRemote"
#define START_SHOOT "startShoot"
#define POS_SYNC "posSync	"
#define CONNECTION_STATE_CHANGED "connectionStateChanged"
#define LAYER_QUIT "layerQuit"
#define GAME_QUIT "gameQuit"

//SOUNDS


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define BALL_SOUND "ball.ogg"
	#define CROWD_SOUND "crowd.ogg"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    #define BALL_SOUND "ball.wav"
	#define CROWD_SOUND "crowd.wav"
#else
    #define BALL_SOUND "ball.wav"
	#define CROWD_SOUND "crowd.wav"
#endif // CC_PLATFORM_WIN32


//FILES

#define USER_DATA_NAME "player_name_key"
#define GOALS_DATA_NAME "prefered_goals"

typedef enum _OpponentType
{
    kHuman = 0,
    kHumanRemote = 1,
    kAI = 2,
    kHumanRemoteReconnect = 3
} OpponentType;

typedef enum _ZOrder
{
    zBackground = 0,
    zGameLevel = 1,
    zOverlay1 = 15,
    zOverlay2 = 16,
    zControls = 17
} ZOrder;

namespace SoccerGlobal
{
	class Settings
	{
		public:
			static Settings& getInstance()
			{
				static Settings instance; // Guaranteed to be destroyed.
									  // Instantiated on first use.
				return instance;
			}

			CC_SYNTHESIZE(float, _xOffset, XOffset);
			CC_SYNTHESIZE(float, _yOffset, YOffset);

			CC_SYNTHESIZE(float, _terrainHeight, TerrainHeight);
			CC_SYNTHESIZE(float, _terrainWidth, TerrainWidth);

			void scaleSprite(CCSprite* sprite);
            void scaleSprite(CCSprite* sprite, float constantScale);
        
        CC_SYNTHESIZE(GameOptions, _options, Options);

		private:
			Settings() {};                   // Constructor? (the {} brackets) are needed here.
			// Dont forget to declare these two. You want to make sure they
			// are unaccessable otherwise you may accidently get copies of
			// your singleton appearing.
			Settings(Settings const&); // Don't Implement
			void operator=(Settings const&); // Don't implement
	};

	typedef enum _TerrainPosition {
		kLeft,
		kRight
	} TerrainPosition;

	//Footballer arrangament relative to the terrainlength/2
	float const firstRowRatio = 0.153846153f;
	float const secondRowRatio = 0.37f;
	float const thirdRowRatio = 0.65f;
	
	float const TerrainAspectRatio = 0.7;

    //Ball radius / terrain height
    float const ballRatio = 0.6777777f;
    float const footballerRatio = 0.95f;

	//Initial offsets
	int const XOffset = 30;
	int const YOffset = 11;
	int const MARGIN_TICKNESS = 2.5;

    float const qZone1XRatio = 0.07f;
	float const qZone1YRatio = 0.05f;
    
	float const qZone2YRatio = 0.1f;
	float const qZone2XRatio = 0.19f;
    
	//Sprites & resource lists
    char* const mainMenuListName = (char*)"main.plist";
    char* const mainMenuImgName = (char*)"main.png";
    
    char* const stadionListName = (char*)"ball.plist";
    char* const stadionImgName = (char*)"ball_sheet.png";
    
	const std::string SETTINGS_BG_NAME = "OptionsBg.png";

    //Fonts
	const std::string S_F_TYPE = 
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	"Marker Felt";
	#else
	"cocosgui/Marker Felt.ttf";
	#endif

}

#endif // End __Soccer_Global__