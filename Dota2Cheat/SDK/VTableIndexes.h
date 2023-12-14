#pragma once

#define Index constexpr uint32_t
namespace VTableIndexes {
	namespace CEngineClient {
		// after GetMaxClients(which returns 64/0x40)
		// either bruteforce or compare which vfunc of xmmword_XXXXXXXX the function called last time and compare with new data
		Index IsInGame = 31;
	}
	namespace CParticleCollection {
		// xref: "modifier_item_ward_true_sight", appears twice in the same func
		// decompile -> under "particles/ui_mouseactions/range_display.vpcf" will be a sub_XXXXXXX call with 3 args
		// that is CNewParticleEffect::SetControlPoint, which gets the CParticleCollection at 0x20 and calls the vfunc at index 16
		Index SetControlPoint = 16;
		// xref: "Error in child list of particle system %s [%p]"
		// easy to see
		Index SetRenderingEnabled = 93;
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
		Index OnWearablesChanged = 263;

		// JS functions
		Index IsRoshan = 60;
		Index GetIdealSpeed = 238;
		Index GetAttackRange = 283;
		Index GetPhysicalArmorValue = 286;
		Index GetMagicalArmorValue = GetPhysicalArmorValue + 1;
	}
	namespace CDOTABaseAbility {
		// JS
		Index GetEffectiveCastRange = 261;
	}
	namespace CDOTA_Buff {
		Index OnAddModifier = 42;
	}
	namespace CUIEngineSource2 {
		// xref: "CUIEngine::RunFrame"
		Index RunFrame = 6;
		// 7th above vfunc with "CUIEngine::RegisterEventHandler"
		// look for the one that matches the dylib version
		Index IsValidPanelPointer = 33;
		// xref: "CUIEngine::RunScript (compile+run)"
		Index RunScript = 81;
	}
	namespace Source2Client {
		// 8 funcs above xrefs "reason_code", "client_disconnect"
		Index VoiceReliable = 105;
		// xrefs: "userid", "steamid", "player_info"
		Index NotifyDisconnect = 14;
	}
}

#undef Index