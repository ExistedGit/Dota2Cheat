#pragma once

#define Index constexpr uint32_t
namespace VTableIndexes {
	namespace CEngineClient {
		Index IsInGame = 30;
		Index GetLocalPlayer = 20;
	}
	namespace CParticleCollection {
		Index SetControlPoint = 7;
	}
	namespace CDOTABaseAbility {
		Index GetCastRangeBonus = 249;
	}
}

#undef Index