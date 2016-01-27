#ifndef __SoccerWorld__
#define __SoccerWorld__

#include "GLES-Render.h"
#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "Box2D/Box2D.h"
#else
#include "Box2D/Box2D.h"
#endif

class StadionLayer;
USING_NS_CC;

class SoccerWorld : public b2World {
    
public:
    
	~SoccerWorld();
    SoccerWorld (StadionLayer * game, b2Vec2 gravity);

	bool initWorld();

    CC_SYNTHESIZE(b2Body *, _body, Body);
    CC_SYNTHESIZE(StadionLayer *, _game, Game);
    
private:
	GLESDebugDraw m_debugDraw;

    b2ContactListener * _collisionListener;
};


#endif /* defined(GLESDebugDraw) */
