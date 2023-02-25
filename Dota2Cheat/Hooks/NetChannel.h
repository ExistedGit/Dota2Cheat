#pragma once
#include "../SDK/INetChannel.h"
#include "../Signatures.h"
#include "VMT.h"
#include "../ShakerAttackAnimFix.h"

namespace Hooks {
	// for MinHook
	inline Signatures::DispatchPacketFn oDispatchPacket = nullptr;
	inline Signatures::BAsyncSendProtoFn oBAsyncSendProto = nullptr;

	inline bool hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID) {
#ifdef _DEBUG
		std::cout << "GCClient Send: " << std::dec << EDOTAGCMsg2String(protobufMsg->msgID) << '\n';
#endif // _DEBUG
		return oBAsyncSendProto(protobufMsg, handler, responseMsg, respMsgID);
	}

	inline bool hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket) {
#ifdef _DEBUG
		std::cout << "GCClient Recv: " << std::dec << EDOTAGCMsg2String(netPacket->GetEMsg()) << '\n';
#endif // _DEBUG
		return oDispatchPacket(thisptr, netPacket);
	};

	inline void hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
		NetMessageInfo_t* info = Interfaces::NetworkMessages->GetNetMessageInfo(messageHandle);
		const char* name = info->pProtobufBinding->GetName();

		Modules::ShakerAttackAnimFix.ChangeAttackAnimIfNeeded(messageHandle, msg);

		return VMTs::NetChannel->GetOriginalMethod<decltype(&hkPostReceivedNetMessage)>(86)(thisptr, messageHandle, msg, type, bits);
	}

	inline void* CreateNetChannel(void* thisptr, int unk, void* ns_addr, const char* str, unsigned int uUnk, unsigned int uUnk2) {
		VMTs::NetChannel.reset();
		void* ret = VMTs::NetworkSystem->GetOriginalMethod<decltype(&CreateNetChannel)>(26)(thisptr, unk, ns_addr, str, uUnk, uUnk2);

		VMTs::NetChannel = std::unique_ptr<VMT>(new VMT(ret));
		//VMTs::NetChannel->HookVM(SendNetMessage, 70);
		VMTs::NetChannel->HookVM(hkPostReceivedNetMessage, 86);
		VMTs::NetChannel->ApplyVMT();

		return ret;
	}
}