//
//  ConectionManager.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 8/14/13.
//
//

#include "ConectionManager.h"
#include "TerrainMap.h"

ConnectionManager::~ConnectionManager()
{
    
}

void ConnectionManager::sendEvent(const NetworkData& data, EventType eventType)
{
	ExitGames::Common::Hashtable table;
    
	std::map<int, float>* map = data.getData();
    
	std::map<int, float>::const_iterator it;
	for (it = map->begin(); it != map->end(); ++it)
	{
		table.put<int, float>(it->first, it->second);
	}
	
    
    NetworkLogic::sendEvent(table, eventType);
}

void ConnectionManager::callForEvent(ExitGames::Common::Hashtable& data, EventType eventType)
{
	if (eventType == kShoot) handleShoot(data);
    if (eventType == kPositionSync) handlePositionSync(data);
}

void ConnectionManager::handleShoot(ExitGames::Common::Hashtable& eventContent)
{
    b2Vec2 speed = getVectorDataFromHash(eventContent);
    
    ObserverData* ob = new ObserverData();
    ob->setSpeed(speed);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(START_SHOOT, ob);
}

void ConnectionManager::handlePositionSync(ExitGames::Common::Hashtable& eventContent)
{
    b2Vec2 speed = getVectorDataFromHash(eventContent);
    
    float x = getFoatValueForParam(kBallPosX, eventContent);
    float y = getFoatValueForParam(kBallPosY, eventContent);
    
    CCPoint pos = ccp(x, y);
    
    ObserverData* ob = new ObserverData();
    
    CCPoint p = TerrainMap::getInstance().getPositionFromRelativeCoordinates(ccp(pos.x, pos.y));
    
    ob->setInitialPosition(p);
    ob->setSpeed(speed);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(POS_SYNC, ob);
}

void ConnectionManager::broadcastRoomStateChanged(EventType evt, ConnectionRelatedData data)
{
    ObserverData* ob = new ObserverData();
    ob->setRemotePlayerName(data.PlayerName);
    ob->setRoom(data.RoomName);
    
    ob->setPlayerIsConnected(evt == evtPlayerConnected);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(CONNECTION_STATE_CHANGED, ob);
}

b2Vec2 ConnectionManager::getVectorDataFromHash(ExitGames::Common::Hashtable& eventContent)
{
    float x = getFoatValueForParam(kTransferDataX, eventContent);
    float y = getFoatValueForParam(kTransferDataY, eventContent);
    
    b2Vec2 vector = b2Vec2(x, y);
    
    return vector;
}

