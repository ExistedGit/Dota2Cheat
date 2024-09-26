#pragma once
#include "../Base/VClass.h"
#include "ISteamGameCoordinator.h"

using HSteamPipe = int32_t;
using HSteamUser = int32_t;

// from steamworks sdk
class ISteamClient : public VClass {
public:
	auto GetISteamGenericInterface(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) {
		return CallVFunc<12, ISteamGameCoordinator*>(hSteamUser, hSteamPipe, pchVersion);
	}
};