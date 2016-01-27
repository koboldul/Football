#ifndef __OUTPUT_LISTENER_H
#define __OUTPUT_LISTENER_H


#include <iostream>
#include "NetworkData.h"
#include "LoadBalancing-cpp/inc/Client.h"

using namespace std;

class OutputListener
{
public:
	virtual ~OutputListener(void){};
    
	virtual void write(const ExitGames::Common::JString& str) = 0;
	
    virtual void writeLine(const ExitGames::Common::JString& str) = 0;
    
    virtual void callForEvent(ExitGames::Common::Hashtable& data, EventType eventType) = 0;
    
    virtual void broadcastRoomStateChanged(EventType evt, ConnectionRelatedData connData) = 0;
    
	virtual void debugReturn(const ExitGames::Common::JString& str)
	{
		
	}
};

#endif
