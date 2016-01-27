#include "pch.h"
#include "SoccerWorld.h"
#include "MyContactListener.h"

using namespace cocos2d;

SoccerWorld::~SoccerWorld() {   
	delete _collisionListener;
    _collisionListener = NULL;
}

SoccerWorld::SoccerWorld (StadionLayer * game, b2Vec2 gravity) : b2World (gravity) {
	_game = game;
}

bool SoccerWorld::initWorld()
{
	SetAllowSleeping(true);
    SetContinuousPhysics(true);

	_collisionListener = new MyContactListener();
	SetContactListener(_collisionListener);

	this->SetDebugDraw(&m_debugDraw);

    //m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    //SetDebugDraw(m_debugDraw);

   // uint32 flags = 0;
    //flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);

	return true;
}
