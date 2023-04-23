#include "OnAcceptMatch.h"
#include "../CheatSDK/Config.h"

void Hooks::RunAcceptTimer() {
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::seconds(Config::AutoAccept::Delay));
	Signatures::CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby(GameSystems::GCClientSystem, true);
		}).detach();
}
void Hooks::hkRunScript(VClass* thisptr, VClass* panel, const char* entireJSCode, const char* pathToXMLContext, int int1, int int2, bool alreadyCompiled) {
	if (Config::AutoAccept::Enabled &&
		Signatures::CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby &&
		strstr(pathToXMLContext, "popup_accept_match.xml")) {
		std::cout << "Trying to accept match\n";
		RunAcceptTimer();
	}

	oRunScript(thisptr, panel, entireJSCode, pathToXMLContext, int1, int2, alreadyCompiled);
}
