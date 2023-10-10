#include "AutoAccept.h"
#include "../../SDK/Protobufs/dota_gcmessages_client_match_management.pb.h"

//void Modules::M_AutoAccept::SetNotificationGameMode(const char* gm) {
//	notificationGameMode = gm;
//}
//
//void Modules::M_AutoAccept::SendTGNotification() {
//	const std::string URL =
//		"https://dota2cheat-bot-api.glitch.me/notify" +
//		std::format("?chat_id={}&delay={}&cheat={}&gamemode={}",
//			Config::API::TelegramID,
//			Config::M_AutoAccept::Delay,
//			"Dota2Cheat",
//			encodeURI(notificationGameMode)
//		);
//	std::stringstream out;
//	CURL* curl = curl_easy_init();
//	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteRemoteString);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
//	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
//	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dota2Cheat/0.1");
//	CURLcode CURLresult = curl_easy_perform(curl);
//	curl_easy_cleanup(curl);
//}

void Modules::M_AutoAccept::RunAcceptTimer() {
	std::this_thread::sleep_for(std::chrono::seconds(Config::AutoAccept::Delay));
	// All that rebuilt from CDOTAGCClientSystem::SendReadyUpMessageForCurrentLobby(), see signatures.json
	auto key = Interfaces::GCClient->GetReadyUpKey();

	CMsgReadyUp msg;
	msg.set_ready_up_key(key);
	Log(LP_INFO, "Accepting match...");
	msg.set_state(DOTALobbyReadyState_ACCEPTED);

	Interfaces::SteamGC->SendMsg(msg, k_EMsgGCReadyUp);

	acceptingMatch = false;
}

void Modules::M_AutoAccept::AcceptMatch() {
	if (!Config::AutoAccept::Enabled ||
		acceptingMatch)
		return;
	acceptingMatch = true;
	//if (Config::M_AutoAccept::SendTelegramNotifications && Config::API::TelegramID != 0)
	//	std::thread(&AutoAccept::SendTGNotification, this).detach();

	std::thread(&M_AutoAccept::RunAcceptTimer, this).detach();
}
