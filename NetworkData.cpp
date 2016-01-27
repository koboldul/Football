#include "NetworkData.h"

NetworkData::NetworkData()
{
	_data = new std::map<int, float>();
}

NetworkData::~NetworkData()
{
	_data = NULL;
	delete _data;
}