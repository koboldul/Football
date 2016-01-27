/*
 * Defines the actual player - computer, human.	
 */

#ifndef __RemoteControl__
#define __RemoteControl__

#include "SoccerGlobal.h"
#include "PlayerControl.h"
#include "ConnectionManagerFacade.h"
#include "TerrainMap.h"
#include "NetworkData.h"

using namespace SoccerGlobal;

class RemoteControl : public PlayerControl
{

public:
    ~RemoteControl();
	RemoteControl(int id);

    void init();
    void update(CCPoint pos, b2Vec2 speed);
    void gameQuit();
    bool whenActiveTouchAllowed()
    {
        return false;
    }
    
private:
    void runConnection();
    void startShooting(ObserverData* param);
    void transferData(ObserverData* param);
    void opponentConnectionState(ObserverData* param);
    
	NetworkData* _lastData;
};

#endif /* defined(__RemoteControl__) */