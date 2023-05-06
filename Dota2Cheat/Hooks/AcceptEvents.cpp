#include "AcceptEvents.h"

void Hooks::hkRunScript(VClass* thisptr, VClass* panel, const char* entireJSCode, const char* pathToXMLContext, int int1, int int2, bool alreadyCompiled) {
	if(Signatures::CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby &&
		strstr(pathToXMLContext, "popup_accept_match.xml"))
		Modules::AutoAccept.AcceptMatch();
	

	oRunScript(thisptr, panel, entireJSCode, pathToXMLContext, int1, int2, alreadyCompiled);
}

