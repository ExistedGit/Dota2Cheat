#include "AutoAccept.h"
#include <dota_gcmessages_client_match_management.pb.h>

void Modules::M_AutoAccept::RunAcceptTimer() {
	std::this_thread::sleep_for(std::chrono::seconds(Config::AutoAccept::Delay));
	// All that rebuilt from CDOTAGCClientSystem::SendReadyUpMessageForCurrentLobby(), see signatures.json
	auto key = CGCClient::Get()->GetReadyUpKey();

	CMsgReadyUp msg;
	msg.set_ready_up_key(key);
	LogI("Accepting match...");
	msg.set_state(DOTALobbyReadyState_ACCEPTED);

	ISteamGC::Get()->SendMsg(msg, k_EMsgGCReadyUp);

	acceptingMatch = false;
}

void Modules::M_AutoAccept::AcceptMatch() {
	if (!Config::AutoAccept::Enabled ||
		acceptingMatch)
		return;
	acceptingMatch = true;

	std::thread(&M_AutoAccept::RunAcceptTimer, this).detach();
}
