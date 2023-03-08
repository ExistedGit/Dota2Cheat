#pragma once
#include "../SDK/pch.h"
#include "../SDK/Interfaces/Network/INetChannel.h"

#include "../SDK/Globals/Signatures.h"
#include "VMT.h"

#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/ParticleAbilityWarner.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"

namespace Hooks {
	inline INetChannel* NetChan{};
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
	}

	typedef void(__fastcall* PostReceivedNetMessageFn)(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits);
	inline PostReceivedNetMessageFn oPostReceivedNetMessage{};
	inline void hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
		if (messageHandle->messageID != 4) // not CNetMsg_Tick [4]
		{
			NetMessageInfo_t* info = Interfaces::NetworkMessages->GetNetMessageInfo(messageHandle);
			const char* name = info->pProtobufBinding->GetName();

			Modules::ShakerAttackAnimFix.ChangeAttackAnimIfNeeded(messageHandle, msg);
			Modules::LinearProjectileWarner.ProcessLinearProjectileMsg(messageHandle, msg);
			Modules::ParticleAbilityWarner.ProcessParticleMsg(messageHandle, msg);
		}

		return oPostReceivedNetMessage(thisptr, messageHandle, msg, type, bits);
	}

	typedef bool(__fastcall* SendNetMessageFn)(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type);
	inline SendNetMessageFn oSendNetMessage{};
	inline bool hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
		return oSendNetMessage(thisptr, messageHandle, msg, type);
	}

	// Another way to hook NetChan.
	// It's unreliable, since you need to reset and capture the object's VMT and it happens strictly during game load
	// So if you reinject with this hooking method, no NetChannel for you :^(
	//inline void* CreateNetChannel(void* thisptr, int unk, void* ns_addr, const char* str, unsigned int uUnk, unsigned int uUnk2) {
	//	VMTs::NetChannel.reset();

	//	void* ret = VMTs::NetworkSystem->GetOriginalMethod<decltype(&CreateNetChannel)>(26)(thisptr, unk, ns_addr, str, uUnk, uUnk2);

	//	VMTs::NetChannel = std::unique_ptr<VMT>(new VMT(ret));
	//	//VMTs::NetChannel->HookVM(hkSendNetMessage, 69);
	//	VMTs::NetChannel->HookVM(hkPostReceivedNetMessage, 86);
	//	VMTs::NetChannel->ApplyVMT();

	//	return ret;
	//}
}