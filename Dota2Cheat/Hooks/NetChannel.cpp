#include "NetChannel.h"

bool Hooks::hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID) {
#ifdef _DEBUG
	std::cout << "GCClient Send: " << std::dec << EDOTAGCMsg2String(protobufMsg->msgID) << '\n';
#endif // _DEBUG
	return oBAsyncSendProto(protobufMsg, handler, responseMsg, respMsgID);
}

bool Hooks::hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket) {
#ifdef _DEBUG
	std::cout << "GCClient Recv: " << std::dec << EDOTAGCMsg2String(netPacket->GetEMsg()) << '\n';
#endif // _DEBUG
	return oDispatchPacket(thisptr, netPacket);
}

void Hooks::hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
	NetChan = thisptr;
	if (messageHandle->messageID != 4) // not CNetMsg_Tick [4]
	{
		NetMessageInfo_t* info = Interfaces::NetworkMessages->GetNetMessageInfo(messageHandle);
		const char* name = info->pProtobufBinding->GetName();

		Modules::ShakerAttackAnimFix.ChangeAttackAnimIfNeeded(messageHandle, msg);
		Modules::LinearProjectileWarner.ProcessLinearProjectileMsg(messageHandle, msg);
		Modules::ParticleAbilityWarner.ProcessParticleMsg(messageHandle, msg);
	}

	oPostReceivedNetMessage(thisptr, messageHandle, msg, type, bits);
}

bool Hooks::hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {


	//if (messageHandle->messageID == 303) {
	//	CDOTAClientMsg_MapPing myMsg{};
	//	auto loc = myMsg.mutable_location_ping();
	//	loc->set_x(ctx.assignedHero->GetPos().x);
	//	loc->set_y(ctx.assignedHero->GetPos().y);
	//	loc->set_target(-1);
	//	loc->set_direct_ping(false);
	//	loc->set_type(0);
	//	loc->set_ping_source(k_ePingSource_Default);
	//	
	//	//CDOTAClientMsg_MapPing* clone = static_cast<CDOTAClientMsg_MapPing*>(msg->New());
	//	//clone->CopyFrom(*static_cast<CDOTAClientMsg_MapPing*>(msg));
	//	//clone->mutable_location_ping()->CopyFrom(*static_cast<CDOTAClientMsg_MapPing*>(msg)->mutable_location_ping());
	//	auto handle = Interfaces::NetworkMessages->FindNetworkMessage2("CDOTAClientMsg_MapPing");
	//	//if (Hooks::NetChan)
	//	
	//	return oSendNetMessage(NetChan, handle, &myMsg, BUF_DEFAULT);
	//}

	return oSendNetMessage(thisptr, messageHandle, msg, type);
}
