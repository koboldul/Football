#ifndef __NetworkData_H
#define __NetworkData_H

#include <cocos2d.h>

typedef enum _EventParam {
    kTransferDataX = 1,
    kTransferDataY = 2,
    kGameState = 3,
    kBallPosX = 4,
    kBallPosY = 5,
    evpPlayerName = 6
    
} EventsParam;

typedef enum _EventType {
    kShoot = 1,
    kGameStateChanged = 2,
    kConnectionStateChanged = 3,
    kPositionSync = 4,
    kReloadState = 5,
    evtPlayerDisconnected = 6,
    evtPlayerConnected = 7,
    evtPassTurn = 8
} EventType;

typedef enum _State
{
	STATE_INITIALIZED = 0,
	STATE_CONNECTING,
	STATE_CONNECTED,
	STATE_JOINING,
	STATE_JOINED,
	STATE_LEAVING,
	STATE_LEFT,
	STATE_DISCONNECTING,
	STATE_DISCONNECTED,
    STATE_CREATED_JOINED
} State;

typedef enum _Input
{
	INPUT_NON = 0,
	INPUT_CREATE_GAME,
	INPUT_JOIN_RANDOM_GAME,
	INPUT_LEAVE_GAME,
	INPUT_EXIT,
    INPUT_RECONNECT
} Input;

struct ConnectionRelatedData
{
    std::string PlayerName;
    std::string RoomName;
    bool OpponentIsDisconnected;
    bool IAmDisconnected;
};

class NetworkData 
{
public:
	NetworkData();
	~NetworkData();

	std::map<int, float>* getData() const { return _data; };
	void setData(std::map<int, float>* val) { _data = val; };

private:
	std::map<int, float>* _data;
};

#endif