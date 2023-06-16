#pragma once
#include "../SDK/pch.h"
#include "../SDK/Protobufs/dota_gcmessages_client_match_management.pb.h"

namespace Hooks {
	using SendMessageFn = EGCResults(*)(ISteamGameCoordinator*, uint32_t, const void*, uint32_t);
	inline SendMessageFn oSendMessage;

	inline EGCResults hkSendMessage(ISteamGameCoordinator* thisptr, uint32_t unMsgType, const void* pubData, uint32_t cubData) {
		//auto msgType = (EDOTAGCMsg)((uint16_t)unMsgType);
		//if (msgType == k_EMsgGCReadyUp) {
		//	CMsgReadyUp msg;
		//	msg.ParseFromArray(pubData, cubData);
		//	
		//	LogF(LP_INFO, "ReadyUp: key {}", msg.ready_up_key());
		//}
		return oSendMessage(thisptr, unMsgType, pubData, cubData);
	}
}