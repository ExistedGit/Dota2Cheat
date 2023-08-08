#pragma once
#include "../pch.h"
#include "../CheatSDK/include.h"
#include "../Modules/Hacks/SkinChanger.h"

// UNUSED, see SteamGC.h for GC hooks
// Game Coordinator is what manages out-of-game activities in Valve games, such as
// items, trading, finding and connecting to a game, Dota+ etc
// Hooking these functions allows us to override GC's network interactions and e.g. change items locally

namespace Hooks {
	inline Signatures::DispatchPacketFn oDispatchPacket = nullptr;
	inline Signatures::BAsyncSendProtoFn oBAsyncSendProto = nullptr;

	bool hkBAsyncSendProto(CProtobufMsgBase<>* protobufMsg, IProtoBufSendHandler* handler);
	bool hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket);	
}