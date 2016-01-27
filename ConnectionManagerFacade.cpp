#include "ConnectionManagerFacade.h"

void ConnectionManagerFacade::sendEvent(const NetworkData& data, EventType eventType)
{
	_manager->sendEvent(data, eventType);
}

State ConnectionManagerFacade::getState(void) const
{
	return _manager->getState();
}

void ConnectionManagerFacade::run(void)
{
	_manager->run();
}

void ConnectionManagerFacade::setLastInput(Input newInput)
{
	_manager->setLastInput(newInput);
}

void ConnectionManagerFacade::disconnect(void)
{
	_manager->disconnect(); 
}