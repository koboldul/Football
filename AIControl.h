/*
 * Defines the actual player - computer, human.	
 */

#ifndef __AIControl__
#define __AIControl__

#include "SoccerGlobal.h"
#include "PlayerControl.h"
#include "TerrainMap.h"

class Gate;

using namespace SoccerGlobal;

class AIControl : public PlayerControl
{

public:
    ~AIControl();
	AIControl(int id);

    void setIsActive(bool isActive);
    void init();
    void update(CCPoint pos, b2Vec2 speed);
    
    bool whenActiveTouchAllowed()
    {
        return false;
    };
    
protected:
    
    void startShooting(ObserverData* param);
    
private:
    
    b2Vec2 getShootData();
	b2Vec2 getAlternate();
	bool isHittingAPlayer(CCPoint p1, CCPoint p2, CCArray* team);
	
    
	CCPoint _lastBallPosition;
    CCPoint _gateLower;
	CCPoint _gateUpper;

    float randomFloat(float a, float b) {
        float random = ((float) rand()) / (float) RAND_MAX;
        float diff = b - a;
        float r = random * diff;
        return a + r;
    }

	double getAngleForPoints(CCPoint p1, CCPoint p2)
	{
		return atan2f(p1.y-p2.y, p1.x-p2.x);
	}

	b2Vec2 getSpeedFromPoints(CCPoint p1, CCPoint p2)
	{
		double angleU = getAngleForPoints(p2, p1);
	
		float powa = ccpDistance(p1, p2);
		b2Vec2 speed = b2Vec2(powa * cos(angleU), powa * sin(angleU));

		return speed;
	}
};

#endif /* defined(__AIControl__) */