#pragma once
#include "../../CheatSDK/include.h"
#include "../MListeners.h"

namespace Modules {
inline class AegisSnatcher : public IFrameListener, IEntityListListener {
		CBaseEntity* aegis = nullptr;
		float lastPickupTime = 0;
	public:
		void OnEntityRemoved(const EntityWrapper& ent) override;
		void OnFrame() override;

		// Inherited via IEntityListListener
		void OnEntityAdded(const EntityWrapper& ent) override;
} AegisSnatcher;
}