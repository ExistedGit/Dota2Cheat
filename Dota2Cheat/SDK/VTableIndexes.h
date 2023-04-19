#pragma once

#define Index constexpr uint32_t
namespace VTableIndexes {
	namespace CEngineClient {
		// after GetMaxClients(which returns 64/0x40)
		// I bruteforced that
		Index IsInGame = 30;
		Index GetLocalPlayer = 20;
	}
	namespace CParticleCollection {
		// xref: "modifier_item_ward_true_sight", appears twice in the same func
		// decompile -> under "particles/ui_mouseactions/range_display.vpcf" will be a sub_XXXXXXX call with 3 args
		// that is CNewParticleEffect::SetControlPoint, which gets the CParticleCollection at 0x20 and calls the vfunc at index 16
		Index SetControlPoint = 16;
	}
	namespace CDOTAParticleManager {
		// JS xref "CreateParticle" to lea RCX
		// Second call is GetParticleManager
		// Third calls a virtual function of RAX, the index is there
		Index CreateParticle = 9;
	}
	namespace CDOTABaseNPC {
		// JS functions
		Index GetPhysicalArmorValue = 271;
		Index GetMagicalArmorValue = 272;
	}
	namespace CDOTABaseAbility {
		Index GetCastRangeBonus = 0X7C8/8;
	}
	namespace CDOTA_Buff {
		Index OnAddModifier = 39;
	}
	namespace CUIEngineSource2 {
		//xref: "CUIEngineSource::RunFrame"
		Index RunFrame = 6;
		// 7th above vfunc with "CUIEngine::RegisterEventHandler"
		// look for the one that matches the dylib version
		Index IsValidPanelPointer = 33;
	}
}

#undef Index