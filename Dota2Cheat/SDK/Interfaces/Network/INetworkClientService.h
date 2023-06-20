#pragma once
#include "../../Base/VClass.h"
#include "../../Base/Definitions.h"
#include "CNetworkGameClient.h"
#include "CNetworkMessages.h"

class INetworkClientService : VClass
{
public:
	GETTER(CNetworkGameClient*, GetIGameClient, 0xb8);
};