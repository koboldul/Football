#ifndef __ConnectionManagerFacade_H
#define __ConnectionManagerFacade_H

#include <cocos2d.h>
#include "ConectionManager.h"
#include "NetworkData.h"

#define sgl_ConnectionManager ConnectionManagerFacade::getInstance()

using namespace cocos2d;

class ConnectionManagerFacade
{
public:
	
    static ConnectionManagerFacade& getInstance()
    {
        static ConnectionManagerFacade instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
	State getState(void) const;
	void sendEvent(const NetworkData& data, EventType eventType);
	void run(void);
	void setLastInput(Input newInput);
	void disconnect(void);
    
    CC_SYNTHESIZE(std::string, _roomNumber, RoomNumber);
    
private:
    ConnectionManagerFacade()
	{
		_roomNumber = "";
		_manager = new ConnectionManager();
	};
    
    ConnectionManagerFacade(ConnectionManagerFacade const&); // Don't Implement
    void operator=(ConnectionManagerFacade const&); // Don't implement
    
    void runConnection();

	ConnectionManager* _manager;
};

#endif