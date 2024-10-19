#pragma once
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_AutoAccept {
		bool acceptingMatch;
	public:
		void RunAcceptTimer();
		void AcceptMatch();
	} AutoAccept{};
}