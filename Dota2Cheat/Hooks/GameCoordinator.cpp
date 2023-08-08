#include "GameCoordinator.h"

bool Hooks::hkBAsyncSendProto(CProtobufMsgBase<>* protobufMsg, IProtoBufSendHandler* handler) {
	// there was the item equipment logic
	// now it's someplace else
	return oBAsyncSendProto(protobufMsg, handler);
}

bool Hooks::hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket) {
	return oDispatchPacket(thisptr, netPacket);
}
