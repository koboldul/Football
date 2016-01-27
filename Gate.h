#ifndef __Gate__
#define __Gate__

#include "b2Sprite.h"
#include "SoccerGlobal.h"

#include "StadionLayer.h"


class Gate : public b2Sprite {

public:
    ~Gate();
	Gate(StadionLayer * game, int type);
	
    CC_SYNTHESIZE(bool, _hasGOAL, HoldsBall);
	CC_SYNTHESIZE(int, _type, Type);
    
    static Gate* create(StadionLayer * game, int type);
    
    
private:
    void initGate(StadionLayer * game);
    CCPoint _startPosition;
	
};

#endif /* defined(__Gate__) */
