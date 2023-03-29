#include "OnAcceptMatch.h"
#include "../Config.h"

void Hooks::RunTimer(VClass** Panel) {
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::seconds(Config::AutoAccept::Delay));
	Signatures::OnAcceptMatch(Panel);
		}).detach();
}
void Hooks::hkRunScript(VClass* thisptr, VClass* panel, const char* entireJSCode, const char* pathToXMLContext, int int1, int int2, bool alreadyCompiled) {
	if (Config::AutoAccept::Enabled &&
		Signatures::OnAcceptMatch &&
		strstr(pathToXMLContext, "popup_accept_match.xml")) {
		std::cout << "Trying to accept match\n";
		RunTimer(&panel);
	}

	oRunScript(thisptr, panel, entireJSCode, pathToXMLContext, int1, int2, alreadyCompiled);
}
