#pragma once

#define Index constexpr uint32_t
namespace VTableIndexes {
	namespace CEngineClient {
		Index IsInGame = 30;
		Index GetLocalPlayer = 20;
	}
	namespace CParticleCollection {
		Index SetControlPoint = 17;
	}
	namespace CDOTAParticleManager {
		// JS xref "CreateParticle" to lea RCX
		// Second call is GetParticleManager
		// Third calls a virtual function of RAX, the index is there
		Index CreateParticle = 9;
	}
	namespace CDOTABaseAbility {
		Index GetCastRangeBonus = 249;
	}
	namespace CDOTA_Buff {
		Index OnAddModifier = 39;
	}
}

#undef Index