#pragma once
#include "../pch.h"

struct IFrameListener {
	virtual void OnFrame() = 0;
};
struct INetChanListener {
	virtual void OnReceivedMsg(NetMessageHandle_t* messageHandle, google::protobuf::Message* msg) = 0;
};