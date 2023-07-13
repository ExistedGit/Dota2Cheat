#pragma once
#include "../pch.h"
#include "../SDK/Protobufs/dota_gcmessages_client_match_management.pb.h"

namespace Hooks {
	using SendMessageFn = EGCResults(*)(ISteamGameCoordinator*, uint32_t, const void*, uint32_t);
	inline SendMessageFn oSendMessage{};
	inline void* oRetrieveMessage{};
	EGCResults hkSendMessage(ISteamGameCoordinator* thisptr, uint32_t unMsgType, const void* pubData, uint32_t cubData);
	EGCResults hkRetrieveMessage(ISteamGameCoordinator* thisptr, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
}