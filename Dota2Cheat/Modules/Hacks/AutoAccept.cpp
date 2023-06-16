#include "AutoAccept.h"

void Hacks::AutoAccept::SetNotificationGameMode(const char* gm) {
	notificationGameMode = gm;
}

void Hacks::AutoAccept::SendTGNotification() {
	const std::string URL =
		"https://dota2cheat-bot-api.glitch.me/notify" +
		std::format("?chat_id={}&delay={}&cheat={}&gamemode={}",
			Config::API::TelegramID,
			Config::AutoAccept::Delay,
			"Dota2Cheat",
			encodeURI(notificationGameMode)
		);
	std::stringstream out;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteRemoteString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dota2Cheat/0.1");
	CURLcode CURLresult = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}

void Hacks::AutoAccept::RunAcceptTimer() {
	std::this_thread::sleep_for(std::chrono::seconds(Config::AutoAccept::Delay));
	Signatures::CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby(GameSystems::GCClientSystem, true);
	acceptingMatch = false;
}

void Hacks::AutoAccept::AcceptMatch() {
	if (!Config::AutoAccept::Enabled ||
		acceptingMatch)
		return;
	acceptingMatch = true;
	Log(LP_INFO, "Trying to accept match");
	if (Config::AutoAccept::SendTelegramNotifications && Config::API::TelegramID != 0)
		std::thread(&AutoAccept::SendTGNotification, this).detach();

	std::thread(&AutoAccept::RunAcceptTimer, this).detach();
}
