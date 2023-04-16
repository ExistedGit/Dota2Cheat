#pragma once
#include "../SDK/pch.h"
#include "../SDK/Interfaces/Network/INetChannel.h"

#include "../SDK/Globals/Signatures.h"

#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Utility/AttackAnimTracker.h"
#include "../Modules/Hacks/ParticleAbilityWarner.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/SkinChanger.h"
#include "../Modules/Hacks/TPTracker.h"

namespace Hooks {
	inline INetChannel* NetChan{};
	// for MinHook
	inline Signatures::DispatchPacketFn oDispatchPacket = nullptr;
	inline Signatures::BAsyncSendProtoFn oBAsyncSendProto = nullptr;

	bool hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID);
	bool hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket);

	typedef void(__fastcall* PostReceivedNetMessageFn)(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits);
	inline PostReceivedNetMessageFn oPostReceivedNetMessage{};
	void hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits);

	typedef bool(__fastcall* SendNetMessageFn)(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type);
	inline SendNetMessageFn oSendNetMessage{};
	bool hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type);

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