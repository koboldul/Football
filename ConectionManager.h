//
//  ConectionManager.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 8/14/13.
//
//

#ifndef __ZombieSoccer__ConectionManager__
#define __ZombieSoccer__ConectionManager__

#include "NetworkLogic.h"
#include "SoccerGlobal.h"
#include "cocos2d.h"
#include "ObserverData.h"
#include "NetworkData.h"

using namespace cocos2d;

class ConnectionManager: public NetworkLogic, public OutputListener, public NetworkLogicListener
{
public:
	
    ConnectionManager(): NetworkLogic(this, L"1.0")
	{
		registerForStateUpdates(this);
	}
   
    ~ConnectionManager();
    
	void write(const ExitGames::Common::JString& str)
	{
        CCLog("WTF : %s", str.UTF8Representation().cstr());
	}
	
    void writeLine(const ExitGames::Common::JString& str)
	{
        CCLog("WTF : %s", str.UTF8Representation().cstr());
    }
    
    void stateUpdate(State newState)
	{
        CCLog("New state : %i", newState);
	}
    
    void callForEvent(ExitGames::Common::Hashtable& data, EventType eventType);
    void sendEvent(const NetworkData& data, EventType eventType);

private:
    
    void handleShoot(ExitGames::Common::Hashtable& eventContent);
    void handlePositionSync(ExitGames::Common::Hashtable& eventContent);
    
    void broadcastRoomStateChanged(EventType evt, ConnectionRelatedData data);

    b2Vec2 getVectorDataFromHash(ExitGames::Common::Hashtable& eventContent);
    
	float getFoatValueForParam(_EventParam param, ExitGames::Common::Hashtable& eventContent)
    {
        float val = 0;
        
        int fp = param;
        
        if(eventContent.getValue(fp))
            val = ExitGames::Common::ValueObject<float>(eventContent.getValue(fp)).getDataCopy();
        
        return val;
    }

};

/*#include <cocos2d.h>
#include "NetworkData.h"

using namespace cocos2d;

class ConnectionManager
{
public:
	ConnectionManager();
    
	State getState(void) const;
	void sendEvent(const NetworkData& data, EventType eventType);
	void run(void);
	void setLastInput(Input newInput);
	void disconnect(void);
};*/


#endif /* defined(__ZombieSoccer__ConectionManager__) */