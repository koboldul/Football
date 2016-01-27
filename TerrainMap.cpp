//
//  TerrainMap.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 10/18/13.
//
//

#include "TerrainMap.h"

CCPoint TerrainMap::getRelativeCoordinatesFromPoint(CCPoint relpoint)
{
    float th = Settings::getInstance().getTerrainHeight();
    float tw = Settings::getInstance().getTerrainWidth();
    
    float relX = (relpoint.x - Settings::getInstance().getXOffset()) / tw;
    float relY = (relpoint.y - Settings::getInstance().getYOffset()) / th;
    
    return ccp(relX, relY);
}

CCPoint TerrainMap::getPositionFromRelativeCoordinates(CCPoint relpoint)
{
    float th = Settings::getInstance().getTerrainHeight();
    float tw = Settings::getInstance().getTerrainWidth();
    
    float x = (relpoint.x * tw) + Settings::getInstance().getXOffset();
    float y = (relpoint.y * th) + Settings::getInstance().getYOffset();
    
    return ccp(x, y);
}

bool TerrainMap::isPointInPlayField(CCPoint point)
{
    return (point.x < (Settings::getInstance().getTerrainWidth() + Settings::getInstance().getXOffset())
            && 
			point.x > Settings::getInstance().getXOffset())
		&&
		   (point.y < (Settings::getInstance().getTerrainHeight() + Settings::getInstance().getYOffset())
			&&
			point.y > Settings::getInstance().getYOffset());
}