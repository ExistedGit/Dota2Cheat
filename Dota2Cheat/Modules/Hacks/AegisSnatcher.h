#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"
#include "../MListeners.h"

namespace Modules {
inline class AegisSnatcher : public IFrameListener, IEntityListListener {
		CBaseEntity* aegis = nullptr;
		float lastPickupTime = 0;
	public:
		void OnEntityRemoved(const EntityWrapper& ent) override;
		void OnFrame() override;
	} AegisSnatcher;
}