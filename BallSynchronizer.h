//
//  BallSynchronizer.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 11/8/13.
//
//

#ifndef __ZombieSoccer__BallSynchronizer__
#define __ZombieSoccer__BallSynchronizer__

#include "cocos2d.h"
#include "Ball.h"
#include <ObserverData.h>
#include <SoccerGlobal.h>

class BallSynchronizer : public CCObject
{
public:
    
    BallSynchronizer(Ball* ball);
    ~BallSynchronizer();
    
    bool ShouldSync();
    void update (float dt);
    
    void setSynchronizationPoint(CCPoint point);
    CCPoint getSynchronizationPoint();
    
    void setSynchronizationSpeed(b2Vec2 speed);
    b2Vec2 getSynchronizationSpeed();
    
private:
    Ball* _ball;
    
    CCPoint  _syncPoint;
    b2Vec2 _speed;
    
    bool isMovingRight(CCPoint newPoint);
    void onPositionSync(ObserverData* param);
};

#endif /* defined(__ZombieSoccer__BallSynchronizer__) */
