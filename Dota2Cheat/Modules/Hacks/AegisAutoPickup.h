#pragma once
#include "../../SDK/pch.h"

namespace Hacks {
	class AegisAutoPickup {
		CBaseEntity* aegis = nullptr;
	public:
		void RemoveIfAegis(CBaseEntity* ent);
		void FrameBasedLogic();
	};
}
namespace Modules {
	inline Hacks::AegisAutoPickup AegisAutoPickup{};
}