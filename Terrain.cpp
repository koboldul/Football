#include "pch.h"

#include "Terrain.h"


Terrain::Terrain(StadionLayer * game, CCSize gateSize)
{
	float x = game->getTerrainWidth() * 0.5f +  Settings::getInstance().getXOffset() + MARGIN_TICKNESS;
	float y = game->getTerrainHeight() * 0.5f + Settings::getInstance().getYOffset() + MARGIN_TICKNESS;
	
	setGateSize(gateSize);
	_center = ccp(x, y);
}

Terrain* Terrain::createWithSizeAndWorld(CCSize  size, StadionLayer * game, CCSize gateSize) {
    Terrain * terrain = new Terrain(game, gateSize);
	
	if (terrain) {
		terrain->setTerrainSize(size);
		terrain->autorelease();

		float smallSide = (game->getTerrainHeight()- gateSize.height)* 0.5f;

		terrain->x0y0 = ccp(Settings::getInstance().getXOffset(), Settings::getInstance().getYOffset());
		terrain->x0G0 = ccp(Settings::getInstance().getXOffset(), Settings::getInstance().getYOffset() + MARGIN_TICKNESS + smallSide);
		terrain->x0G1 = ccp(Settings::getInstance().getXOffset(), Settings::getInstance().getYOffset() + MARGIN_TICKNESS + smallSide + gateSize.height);
		terrain->x0y1 = ccp(Settings::getInstance().getXOffset(), Settings::getInstance().getYOffset() + 2 * MARGIN_TICKNESS + game->getTerrainHeight());

		terrain->x1y0 = ccp(Settings::getInstance().getXOffset() + MARGIN_TICKNESS * 2 + game->getTerrainWidth(), Settings::getInstance().getYOffset());
		terrain->x1G0 = ccp(Settings::getInstance().getXOffset() + MARGIN_TICKNESS * 2 + game->getTerrainWidth(), Settings::getInstance().getYOffset() + MARGIN_TICKNESS + smallSide);
		terrain->x1G1 = ccp(Settings::getInstance().getXOffset() + MARGIN_TICKNESS * 2 + game->getTerrainWidth(), Settings::getInstance().getYOffset() + MARGIN_TICKNESS + smallSide + gateSize.height);
		terrain->x1y1 = ccp(Settings::getInstance().getXOffset() + MARGIN_TICKNESS * 2 + game->getTerrainWidth(), Settings::getInstance().getYOffset() + 2 * MARGIN_TICKNESS + game->getTerrainHeight());

		terrain->initEdges(game->getWorld());

		return terrain;
	}
	CC_SAFE_DELETE(terrain);
	return NULL;
}

void Terrain::draw() {
    
    glLineWidth(MARGIN_TICKNESS );
    //glBlendColor(0, 0 , 0, 0.1);
    
    ccDrawColor4F(1.0f, 1.0f, 1.0f, 0.025f);

	ccDrawLine(x0y0, x0G0);
    ccDrawLine(x0y0, x1y0);
	ccDrawLine(x1y0, x1G0);

	float qz1Widh = Settings::getInstance().getTerrainWidth() * qZone1XRatio;
	float qz1Height = Settings::getInstance().getTerrainHeight() * qZone1YRatio;

	float qz2Widh = Settings::getInstance().getTerrainWidth() * qZone2XRatio;
	float qz2Height = Settings::getInstance().getTerrainHeight() * qZone2XRatio;

	//QZone1
	ccDrawLine(ccp(x0G0.x, x0G0.y - qz1Height), ccp(x0G0.x + qz1Widh, x0G0.y - qz1Height));
	ccDrawLine(ccp(x0G1.x, x0G1.y + qz1Height), ccp(x0G1.x + qz1Widh, x0G1.y + qz1Height));
	ccDrawLine(ccp(x0G0.x + qz1Widh, x0G0.y - qz1Height), ccp(x0G1.x + qz1Widh, x0G1.y + qz1Height));

	ccDrawLine(ccp(x0G0.x, x0G0.y - qz2Height), ccp(x0G0.x + qz2Widh, x0G0.y - qz2Height));
	ccDrawLine(ccp(x0G1.x, x0G1.y + qz2Height), ccp(x0G1.x + qz2Widh, x0G1.y + qz2Height));
	ccDrawLine(ccp(x0G0.x + qz2Widh, x0G0.y - qz2Height), ccp(x0G1.x + qz2Widh, x0G1.y + qz2Height));	

	ccDrawLine(x0G1, x0y1);
	ccDrawLine(x0y1, x1y1);
	ccDrawLine(x1y1, x1G1);

	//QZone2
	ccDrawLine(ccp(x1G0.x, x1G0.y - qz1Height), ccp(x1G0.x - qz1Widh, x1G0.y - qz1Height));
	ccDrawLine(ccp(x1G1.x, x1G1.y + qz1Height), ccp(x1G1.x - qz1Widh, x1G1.y + qz1Height));
	ccDrawLine(ccp(x1G0.x - qz1Widh, x1G0.y - qz1Height), ccp(x1G1.x - qz1Widh, x1G1.y + qz1Height));

    ccDrawLine(ccp(x1G0.x, x1G0.y - qz2Height), ccp(x1G0.x - qz2Widh, x1G0.y - qz2Height));
	ccDrawLine(ccp(x1G1.x, x1G1.y + qz2Height), ccp(x1G1.x - qz2Widh, x1G1.y + qz2Height));
	ccDrawLine(ccp(x1G0.x - qz2Widh, x1G0.y - qz2Height), ccp(x1G1.x - qz2Widh, x1G1.y + qz2Height));
    
	CCPoint lowerHalf = ccp((x1y0.x - x0y0.x)*0.5f + x0y0.x, x0y0.y);
	CCPoint upperHalf = ccp((x1y0.x - x0y0.x)*0.5f + x0y0.x, x0y1.y);

	ccDrawLine(lowerHalf, upperHalf);
	
	ccDrawCircle(_center, 5, CC_DEGREES_TO_RADIANS(360), 60, false);
	ccDrawCircle(_center, 50, CC_DEGREES_TO_RADIANS(360), 60, false);
	
}

void Terrain::initEdges(b2World* world)
{
	//create table sides
    b2BodyDef tableBodyDef;
    tableBodyDef.position.Set(0, 0);
    b2Body* tableBody = world->CreateBody(&tableBodyDef);
    
    // Define the table edges
    b2EdgeShape tableBox;
    
    // bottom
	tableBox.Set(b2Vec2(x0y0.x / PTM_RATIO, x0y0.y/ PTM_RATIO), b2Vec2(x1y0.x / PTM_RATIO, x1y0.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);

	// top
    tableBox.Set(b2Vec2(x0y1.x / PTM_RATIO, x0y1.y/ PTM_RATIO), b2Vec2(x1y1.x / PTM_RATIO, x1y1.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    // left
    tableBox.Set(b2Vec2(x0y0.x / PTM_RATIO, x0y0.y/ PTM_RATIO), b2Vec2(x0G0.x / PTM_RATIO, x0G0.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    tableBox.Set(b2Vec2(x0G1.x / PTM_RATIO, x0G1.y/ PTM_RATIO), b2Vec2(x0y1.x / PTM_RATIO, x0y1.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
	
	tableBox.Set(b2Vec2((x0G0.x - _gateSize.width)/ PTM_RATIO, x0G0.y/ PTM_RATIO), 
				 b2Vec2((x0G1.x - _gateSize.width)/ PTM_RATIO, x0G1.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    // right
    tableBox.Set(b2Vec2(x1G1.x / PTM_RATIO, x1G1.y/ PTM_RATIO), b2Vec2(x1y1.x / PTM_RATIO, x1y1.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    tableBox.Set(b2Vec2(x1y0.x / PTM_RATIO, x1y0.y/ PTM_RATIO), b2Vec2(x1G0.x / PTM_RATIO, x1G0.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
	tableBox.Set(b2Vec2((x1G0.x + _gateSize.width)/ PTM_RATIO, x1G0.y/ PTM_RATIO), 
				 b2Vec2((x1G1.x + _gateSize.width)/ PTM_RATIO, x1G1.y/ PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
}
    
CCPoint Terrain::getCenter()
{
	return _center;
}