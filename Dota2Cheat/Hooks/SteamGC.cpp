#include "SteamGC.h"
#include "../Modules/Hacks/AutoAccept.h"

EGCResults Hooks::hkSendMessage(ISteamGameCoordinator* thisptr, uint32_t unMsgType, const void* pubData, uint32_t cubData) {
	//const auto msg_id = unMsgType & 0x7FFFFFFF;
	//auto body_data = (void*)((std::uintptr_t)pubData + 8);
	//auto body_size = cubData - 8;
	//if (msg_id == k_EMsgGCReadyUp) {
	//	CMsgReadyUp body;
	//	if (body.ParsePartialFromArray(body_data, body_size)) {
	//		
	//	}
	//}
	return oSendMessage(thisptr, unMsgType, pubData, cubData);
}

EGCResults Hooks::hkRetrieveMessage(ISteamGameCoordinator* thisptr, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize) {
	const auto ret = (decltype(&hkRetrieveMessage)(oRetrieveMessage))(thisptr, punMsgType, pubDest, cubDest, pcubMsgSize);

	const auto msg_id = *punMsgType & 0x7FFFFFFF;
	auto body_data = (void*)(((std::uintptr_t)pubDest) + 8);
	auto body_size = *pcubMsgSize - 8;
	if (msg_id == k_EMsgGCReadyUpStatus) {
		//CMsgReadyUpStatus body;
		//if (body.ParsePartialFromArray(body_data, body_size)) {
		//	static auto steamID = Interfaces::GCClient->GetSOListeners()[1]->GetSOCache()->GetOwner();
		//	auto accepted = body.accepted_ids();
		//	bool hasOwnId = false;
		//	for (auto& id : accepted) {

		//		if (id == steamID.m_unSteamID) {
		//			hasOwnId = true;
		//			break;
		//		}
		//	}
		//	if (!hasOwnId)
		//}
		Modules::AutoAccept.AcceptMatch();
	}

	return ret;
};
