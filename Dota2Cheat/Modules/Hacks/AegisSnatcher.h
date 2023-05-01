#pragma once
#include "../../SDK/pch.h"

namespace Hacks {
	class AegisSnatcher {
		CBaseEntity* aegis = nullptr;
		float lastPickupTime = 0;
	public:
		void RemoveIfAegis(CBaseEntity* ent);
		void FrameBasedLogic();
	};
}
namespace Modules {
	inline Hacks::AegisSnatcher AegisSnatcher{};
}