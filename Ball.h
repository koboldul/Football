#ifndef __Ball__
#define __Ball__

#include "b2Sprite.h"


class Ball : public b2Sprite {

public:
    ~Ball();
	Ball(StadionLayer * game, CCPoint position);

    static Ball* create(StadionLayer * game, CCPoint position);
    
    void reset(void);
    void update (float dt);
    void startShooting(b2Vec2 velocity);
    
    

private:
    void initBall();
    
    CCPoint _startPosition;
    
    CCSpeed * _animateBall;
    
    
};

#endif /* defined(__Soccer__Ball__) */
