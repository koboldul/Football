/*
 * Defines the actual player - computer, human.
 */

#ifndef __ObserverData__
#define __ObserverData__

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "Box2D/Box2D.h"
#else
#include "Box2D/Box2D.h"
#endif

using namespace cocos2d;


class ObserverData : public CCObject
{
    
public:
    ~ObserverData();
	ObserverData();
    
    CC_SYNTHESIZE(b2Vec2, _speed, Speed);
    CC_SYNTHESIZE(CCPoint, _initialPosition, InitialPosition);
    CC_SYNTHESIZE(CCPoint, _finalPosition, FinalPosition);
    

    CC_SYNTHESIZE(bool, _remotePlayerGoesFirst, RemotePlayerGoesFirst);
    
    CC_SYNTHESIZE(string, _remotePlayerName, RemotePlayerName);
    CC_SYNTHESIZE(string, _room, Room);
    CC_SYNTHESIZE(bool, _isConnected, PlayerIsConnected);
};

#endif /* defined(__ObserverData__) */