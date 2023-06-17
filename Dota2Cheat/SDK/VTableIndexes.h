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
		// xref: "m_Children.m_pHead: [%p]\n"
		// the only one in the vtable
		Index SetRenderingEnabled = 92;
	}
	namespace CDOTAParticleManager {
		// JS xref "CreateParticle" to lea RCX
		// Second call is GetParticleManager
		// Third calls a virtual function of RAX, the index is there
		Index CreateParticle = 9;
	}
	namespace CDOTABaseNPC {
		// the name is an xref, decompile what you found
		// the function that gets placed into a variable accepts int64 a1 and calls its vfunc
		// the natural assumption would be that a1 is C_DOTA_BaseNPC*
		Index OnWearablesChanged = 249;

		// JS functions
		Index GetIdealSpeed = 224;
		Index GetAttackRange = 267;
		Index GetPhysicalArmorValue = 270;
		Index GetMagicalArmorValue = 271;
	}
	namespace CDOTABaseAbility {
		Index GetEffectiveCastRange = 257;
	}
	namespace CDOTA_Buff {
		Index OnAddModifier = 42;
	}
	namespace CUIEngineSource2 {
		// xref: "RunFrame"
		Index RunFrame = 6;
		// 7th above vfunc with "CUIEngine::RegisterEventHandler"
		// look for the one that matches the dylib version
		Index IsValidPanelPointer = 33;
	}
}

#undef Index