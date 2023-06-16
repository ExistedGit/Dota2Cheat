#pragma once
#include "../Base/VClass.h"
#include "ISteamGameCoordinator.h"

// from steamworks sdk
class ISteamClient : public VClass {
public:
	auto GetISteamGenericInterface(void* hSteamUser, void* hSteamPipe, const char* pchVersion) {
		return CallVFunc<12, ISteamGameCoordinator*>(hSteamUser, hSteamPipe, pchVersion);
	}
};