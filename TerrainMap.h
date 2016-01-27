//
//  TerrainMap.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 10/18/13.
//
// Singleton class for exposing game elements to the application.
//

#ifndef __ZombieSoccer__TerrainMap__
#define __ZombieSoccer__TerrainMap__

#include "cocos2d.h"
#include "SoccerGlobal.h"
#include "Player.h"

#define sgl_TerrainMap TerrainMap::getInstance()

using namespace cocos2d;
using namespace SoccerGlobal;

class Player;

class TerrainMap
{
public:
    static TerrainMap& getInstance()
    {
        static TerrainMap instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    CC_SYNTHESIZE(Player*, _lplayer, LeftPlayer);
    CC_SYNTHESIZE(Player*, _rplayer, RightPlayer);
    
    CCPoint getRelativeCoordinatesFromPoint(CCPoint point);
    CCPoint getPositionFromRelativeCoordinates(CCPoint relpoint);
    
    void updatePlayerPosition();
    bool isPointInPlayField(CCPoint point);
    
private:
    TerrainMap() {};
    TerrainMap(TerrainMap const&);
    void operator=(TerrainMap const&);
};

#endif /* defined(__ZombieSoccer__TerrainMap__) */
