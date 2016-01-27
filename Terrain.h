#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "Box2D/Box2D.h"
#else
#include "Box2D/Box2D.h"
#endif
#include "SoccerGlobal.h"

#include "StadionLayer.h"

using namespace SoccerGlobal;
USING_NS_CC;

class Terrain : public CCNode 
{
    
public:
	Terrain(StadionLayer * game, CCSize gateSize);
    CC_SYNTHESIZE(CCSize, _terrainSize, TerrainSize);
	CC_SYNTHESIZE(CCSize, _gateSize, GateSize);
	
	static Terrain * createWithSizeAndWorld(CCSize terrainSize, StadionLayer * game, CCSize gateSize);
    
	virtual void draw();
	CCPoint getCenter();
    
private:
	void initEdges(b2World* world);

	CCPoint x0y0;
	CCPoint x0G0;
	CCPoint x0G1;
	CCPoint x0y1;

	CCPoint x1y0;
	CCPoint x1G0;
	CCPoint x1G1;
	CCPoint x1y1;

	CCPoint _center;
};

#endif // __TERRAIN_H__


