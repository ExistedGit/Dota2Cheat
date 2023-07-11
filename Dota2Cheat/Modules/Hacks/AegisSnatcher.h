#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include "../MListeners.h"

namespace Modules {
inline class AegisSnatcher : public IRunFrameListener, IM_EntityListener {
		CBaseEntity* aegis = nullptr;
		float lastPickupTime = 0;
	public:
		void OnEntityRemoved(const EntityWrapper& ent) override;
		void OnFrame() override;
	} AegisSnatcher;
}