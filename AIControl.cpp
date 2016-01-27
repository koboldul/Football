#include "pch.h"

#include "AIControl.h"

AIControl::AIControl(int id) : PlayerControl(id)
{
    setPlayerType(kAI);
}

AIControl::~AIControl() 
{
}

void AIControl::init()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AIControl::startShooting), START_SHOOT, NULL);
    setPlayerName("Abulafia");

	_gateLower = CCPointZero;
	_gateUpper = CCPointZero;
}

void AIControl::setIsActive(bool isActive)
{
    _state.isActive = isActive;
    
    if (_state.isActive)
    {
        CCLog("Player %s is active" , getPlayerName().c_str());
        _state.hasShoot = true;
        
        //sleep(1000);
        //Compute
        b2Vec2 speed = getShootData();
        ObserverData* data = new ObserverData();
        data->setSpeed(speed);
    
        startShooting(data);
        
    }
    else
    {
        _state.hasShoot = false;
    }
}

void AIControl::startShooting(ObserverData* param)
{
	if (0 == _gateLower.y)
	{
		Gate* targetGate = NULL;
    
		if (kRight == getSide())
		{
			targetGate = sgl_TerrainMap.getLeftPlayer()->getGate();
		}
		if (kLeft == getSide())
		{
			targetGate = TerrainMap::getInstance().getRightPlayer()->getGate();
		}

		 CCRect gateSize = targetGate->boundingBox();
    
		_gateLower = ccp(targetGate->getPosition().x + gateSize.size.width / 2,
								targetGate->getPosition().y - gateSize.size.height / 2);
    
		_gateUpper = ccp(targetGate->getPosition().x + gateSize.size.width / 2,
								targetGate->getPosition().y + gateSize.size.height / 2);
	}
    _lastBallPosition = ccp(0,0);

    if (getIsActive())
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(RESOLVE_SHOOT, param);
        _state.hasShoot = false;
    }
}

b2Vec2 AIControl::getShootData()
{
    float y = randomFloat(_gateLower.y, _gateUpper.y);
	b2Vec2 speed = b2Vec2_zero;

	for(int i = _gateLower.y + 11; i<_gateUpper.y - 11; i++)
	{
		CCPoint targetPoint = ccp(_gateLower.x, i);

		if (!isHittingAPlayer(_lastBallPosition, targetPoint, sgl_TerrainMap.getRightPlayer()->getTeam())
			&&
		    !isHittingAPlayer(_lastBallPosition, targetPoint, sgl_TerrainMap.getLeftPlayer()->getTeam()))
		{
			speed = getSpeedFromPoints(_lastBallPosition, targetPoint);

			CCLOG("Streight");
			
			break;
		}
	}

	if (b2Vec2_zero == speed)
	{
		speed = getAlternate();
	}

	if (b2Vec2_zero == speed)
	{
		CCPoint targetPoint = ccp(_gateLower.x, y);
		speed = getSpeedFromPoints(_lastBallPosition, targetPoint);

		CCLOG("Random");
	}

	return b2Vec2(speed.x * 12, speed.y * 12);
}

void AIControl::update(CCPoint pos, b2Vec2 speed)
{
    _lastBallPosition = pos;
}

b2Vec2 AIControl::getAlternate()
{
	b2Vec2 dir = b2Vec2_zero;
	for(int i=_gateLower.y + BALL_RADIUS ; i<_gateUpper.y - BALL_RADIUS ;i++)
	{
		float terY = sgl_Settings.getYOffset() + sgl_Settings.getTerrainHeight();
		float xhit = (_lastBallPosition.x * (terY - i) + _gateLower.x * (terY-_lastBallPosition.y)) /
			(2*terY - _gateLower.y - _lastBallPosition.y);

		CCPoint borderPoint = ccp(xhit, terY);

		if(!isHittingAPlayer(_lastBallPosition, borderPoint, sgl_TerrainMap.getRightPlayer()->getTeam())
			&&
		   !isHittingAPlayer(_lastBallPosition, borderPoint, sgl_TerrainMap.getLeftPlayer()->getTeam()))
		{
			dir = getSpeedFromPoints(_lastBallPosition, borderPoint);
			break;
		} 
	}

	return dir;
}

bool AIControl::isHittingAPlayer(CCPoint p1, CCPoint p2, CCArray* team)
{
	bool willHit = false;

	if (p1.x == p2.x) return willHit;

	float a = (p1.y - p2.y) / (p1.x - p2.x);
	float b = p1.y - p1.x * a;

	for (size_t i=0; i < team->count(); i++)
	{
		b2Sprite* footballer =  (b2Sprite *)(team->objectAtIndex(i));
        
        CCSize s = footballer->boundingBox().size;
        
		float iii = 10;

		if (NULL != footballer)
		{
			float fY = footballer->getPosition().x * a + b;
			double wtf = atan2((p1.y - p2.y), (p1.x - p2.x));
			double proj = cos(wtf) * (fY - footballer->getPosition().y);

			if (proj * proj < (iii + 11) * (iii + 11))
			{
				willHit = true;
				break;
			}
		}
	}

	return willHit;
}