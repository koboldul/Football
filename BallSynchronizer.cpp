//
//  BallSynchronizer.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 11/8/13.
//
//

#include "BallSynchronizer.h"

using namespace cocos2d;
using namespace SoccerGlobal;

BallSynchronizer::BallSynchronizer(Ball* ball)
{
    _ball = ball;
    _syncPoint = CCPointZero;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BallSynchronizer::onPositionSync), POS_SYNC, NULL);
}

BallSynchronizer::~BallSynchronizer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, POS_SYNC);
    
    CCLOG("Destroying the ball synchronizer!");
}

void BallSynchronizer::update(float dt)
{
    if (ShouldSync())
    {
        _ball->setSpritePosition(_syncPoint);
        _ball->getBody()->SetLinearVelocity(b2Vec2(getSynchronizationSpeed().x * 0.7f, getSynchronizationSpeed().y * 0.7f));
        
        _syncPoint = CCPointZero;
    }
    else
    {
        
        _ball->update(dt);
    }
    
}

void BallSynchronizer::setSynchronizationPoint(CCPoint point)
{
    if (!isMovingRight(point) || _syncPoint.equals(CCPointZero))
    {
        _syncPoint = point;
    }
}

CCPoint BallSynchronizer::getSynchronizationPoint()
{
    return _syncPoint;
}

void BallSynchronizer::setSynchronizationSpeed(b2Vec2 speed)
{
    _speed = speed;
}

b2Vec2 BallSynchronizer::getSynchronizationSpeed()
{
    return _speed;
}

bool BallSynchronizer::ShouldSync()
{
    return !_syncPoint.equals(CCPointZero);
}

bool BallSynchronizer::isMovingRight(CCPoint newPoint)
{
    if (newPoint.x * _speed.x / _speed.y == newPoint.y)
    {
        CCLog("Ball syn Moving right");
    }
    return newPoint.x * _speed.x / _speed.y == newPoint.y;
}

void BallSynchronizer::onPositionSync(ObserverData* param)
{
    b2Vec2 speed = param->getSpeed();
    CCPoint pos = param->getInitialPosition();
    
    setSynchronizationPoint(pos);
    setSynchronizationSpeed(speed);
}