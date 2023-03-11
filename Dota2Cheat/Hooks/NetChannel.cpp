#include "NetChannel.h"

bool Hooks::hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID) {
#ifdef _DEBUG
	std::cout << "GCClient Send: " << std::dec << EDOTAGCMsg2String(protobufMsg->msgID) << '\n';
#endif // _DEBUG
#include "../Modules/Utility/AttackAnimTracker.h"
	return oBAsyncSendProto(protobufMsg, handler, responseMsg, respMsgID);
}

bool Hooks::hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket) {
#ifdef _DEBUG
	std::cout << "GCClient Recv: " << std::dec << EDOTAGCMsg2String(netPacket->GetEMsg()) << '\n';
#endif // _DEBUG
	return oDispatchPacket(thisptr, netPacket);
}

void Hooks::hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
	if (messageHandle->messageID != 4) // not CNetMsg_Tick [4]
	{
		NetMessageInfo_t* info = Interfaces::NetworkMessages->GetNetMessageInfo(messageHandle);
		const char* name = info->pProtobufBinding->GetName();

		Modules::ShakerAttackAnimFix.ChangeAttackAnimIfNeeded(messageHandle, msg);
		Modules::LinearProjectileWarner.ProcessLinearProjectileMsg(messageHandle, msg);
		Modules::ParticleAbilityWarner.ProcessParticleMsg(messageHandle, msg);
		Modules::AttackAnimTracker.ProcessAttackAnimMessage(messageHandle, msg);
	}

	return oPostReceivedNetMessage(thisptr, messageHandle, msg, type, bits);
}

bool Hooks::hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
	return oSendNetMessage(thisptr, messageHandle, msg, type);
}
