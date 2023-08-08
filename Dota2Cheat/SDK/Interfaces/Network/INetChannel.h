#pragma once
#include "CNetworkMessages.h"

class INetChannel : public VClass
{
public:
	bool SendNetMessage(NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
		return CallVFunc<69>(messageHandle, msg, type);
	}
};