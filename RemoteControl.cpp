#include "pch.h"

#include "RemoteControl.h"


RemoteControl::RemoteControl(int id) : PlayerControl(id)
{
	_lastData = new NetworkData();
    setPlayerType(kHumanRemote);
}

RemoteControl::~RemoteControl()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(RemoteControl::runConnection), this);
    
    CCLog("Destroyed the remote control");
    
	_lastData = NULL;
	delete _lastData;

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, RESOLVE_SHOOT);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, START_SHOOT);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, CONNECTION_STATE_CHANGED);
}

void RemoteControl::init()
{
	CCLog("IS multiplayer or what?");

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RemoteControl::startShooting), START_SHOOT, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RemoteControl::transferData), RESOLVE_SHOOT, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RemoteControl::opponentConnectionState), CONNECTION_STATE_CHANGED, NULL);
    
    _state.isInitialized = false;
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(RemoteControl::runConnection), this, 0.5f, false);
    
    setPlayerName("RemotePlayer");
}

void RemoteControl::startShooting(ObserverData* param)
{
    if (getIsActive())
    {
        _state.needsSync = false;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(RESOLVE_SHOOT, param);
        CCLog("wtf is going on why no shoot");
        _state.hasShoot = false;
    }
}

void RemoteControl::runConnection()
{
    sgl_ConnectionManager.run();
    
    if (sgl_ConnectionManager.getState() == STATE_CONNECTED)
    {
        sgl_ConnectionManager.setLastInput(INPUT_JOIN_RANDOM_GAME);
    }
    
    if ((sgl_ConnectionManager.getState() == STATE_JOINED || sgl_ConnectionManager.getState() == STATE_CREATED_JOINED)
        &&
         !_state.localConnectionEstablished)
    {
        _state.localConnectionEstablished = true;
        _state.isHost  = sgl_ConnectionManager.getState() == STATE_CREATED_JOINED;
    }
    
    if (sgl_ConnectionManager.getState() == STATE_DISCONNECTED || sgl_ConnectionManager.getState() == STATE_DISCONNECTING)
    {
        _state.isInitialized = false;
        
        sgl_ConnectionManager.setLastInput(INPUT_RECONNECT);
    }
    else{
        if (_state.localConnectionEstablished && _state.remoteConnectionEstablished)
        {
            _state.isInitialized = true;
        }
    }
}

void RemoteControl::opponentConnectionState(ObserverData* param)
{
    _state.remoteConnectionEstablished = param->getPlayerIsConnected();
    
    if (!_state.remoteConnectionEstablished)
    {
        _state.isInitialized = false;
    }
    else
    {
        sgl_ConnectionManager.setRoomNumber(param->getRoom());
        setPlayerName(param->getRemotePlayerName());
    }
}

void RemoteControl::transferData(ObserverData* param)
{
    if (!getIsActive())
    {
        if (sgl_ConnectionManager.getState() == STATE_JOINED || sgl_ConnectionManager.getState() == STATE_CREATED_JOINED)
        {
            NetworkData ndata;
			std::map<int,float>* data = new std::map<int,float>();

			data->insert(std::pair<int,float>(1, param->getSpeed().x * 0.7f));
			data->insert(std::pair<int,float>(2,  param->getSpeed().y * 0.7f));

			ndata.setData(data);
                        
            sgl_ConnectionManager.sendEvent(ndata, kShoot);
            
            _state.needsSync = true;
        }
    }
}

void RemoteControl::update(CCPoint pos, b2Vec2 speed)
{
    if (!getIsActive() && _state.needsSync)
    {
        if (sgl_ConnectionManager.getState() == STATE_JOINED || sgl_ConnectionManager.getState() == STATE_CREATED_JOINED)
        {
            
            CCPoint relp = TerrainMap::getInstance().getRelativeCoordinatesFromPoint(pos);
			
			std::map<int,float>* data = new std::map<int,float>();

			data->insert(std::pair<int,float>(4, relp.x));
			data->insert(std::pair<int,float>(5, relp.y));
			data->insert(std::pair<int,float>(1, speed.x));
			data->insert(std::pair<int,float>(2, speed.x));

			_lastData->setData(data);
            
            sgl_ConnectionManager.sendEvent(*_lastData, kPositionSync);
        }
    }
}

void RemoteControl::gameQuit()
{
    sgl_ConnectionManager.disconnect();
}
