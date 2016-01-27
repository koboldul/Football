#ifndef __MyContactListener__
#define __MyContactListener__

#include <set>
#include <algorithm>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "Box2D/Box2D.h"
#else
#include "Box2D/Box2D.h"
#endif

class MyContactListener : public b2ContactListener {
public:
	
	MyContactListener();
	~MyContactListener();
    
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);   
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif /* defined(__MyContactListener__) */