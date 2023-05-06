#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include <curl/curl.h>
namespace Hacks {
	class AutoAccept {
		std::string notificationGameMode;
		bool acceptingMatch;
	public:
		void SetNotificationGameMode(const char* gm);
		void SendTGNotification();
		void RunAcceptTimer();

		void AcceptMatch();;
	};
}

namespace Modules {
	inline Hacks::AutoAccept AutoAccept{};
}