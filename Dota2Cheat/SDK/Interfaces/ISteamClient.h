#pragma once
#include "../Base/VClass.h"

class ISteamClient : public VClass {
public:
	void* GetISteamGenericInterface(void* hSteamUser, void* hSteamPipe, const char* pchVersion) {
		return CallVFunc<12>(hSteamUser, hSteamPipe, pchVersion);
	}
};