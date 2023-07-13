#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_AutoAccept {
		//std::string notificationGameMode;
		bool acceptingMatch;
	public:
		//void SetNotificationGameMode(const char* gm);
		//void SendTGNotification();
		void RunAcceptTimer();
		void AcceptMatch();
	} AutoAccept{};
}