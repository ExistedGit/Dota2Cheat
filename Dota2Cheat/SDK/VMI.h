#pragma once
#include <cstdint>

// The idea behind dynamically loading VMTs is that the cheat is easier to update and it doesn't require recompiling the SDK
// Paired with an RTTI parser that generates the JSON, this system allows to maintain VM indices a lot easier
// Thus there are a lot less footguns to disarm in the depths of the SDK

// Virtual Method Index
namespace VMI {
#define Index inline uint32_t
	
	// client.dll
	namespace CBaseEntity {
		Index GetSchemaBinding{};
	}

	// client.dll:C_DOTAGamerules
	namespace CDOTAGameRules {
		// JS func
		Index GetGameTime{};
	}

	// client.dll
	namespace CPanel2D {
		// JS func
		Index GetPositionWithinWindow{};
	}

	// panorama.dll
	namespace CUIPanel {
		// JS func in client.dll, refers to (a1 + 8) which is the CUIPanel
		Index BHasClass{};
	}

	// engine2.dll
	namespace CEngineClient {
		// after GetMaxClients(which returns 64/0x40)
		// either bruteforce or compare which vfunc of xmmword_XXXXXXXX the function called last time and compare with new data
		Index IsInGame{};
	}

	// particles.dll
	namespace CParticleCollection {
		// xref: "modifier_item_ward_true_sight", appears twice in the same func
		// decompile -> under "particles/ui_mouseactions/range_display.vpcf" will be a sub_XXXXXXX call with 3 args
		// that is CNewParticleEffect::SetControlPoint, which gets the CParticleCollection at 0x20 and calls the vfunc at index 16
		Index SetControlPoint{};
		// xref: "Error in child list of particle system %s [%p]"
		// easy to see
		Index SetRenderingEnabled{};
	}

	// client.dll
	namespace CDOTAParticleManager {
		// JS xref "CreateParticle" to lea RCX
		// Second call is GetParticleManager
		// Third calls a virtual function of RAX, the index is there
		Index CreateParticle{};
	}

	// client.dll
	namespace CDOTABaseNPC {
		// the name is an xref, decompile what you found
		// the function that gets placed into a variable accepts int64 a1 and calls its vfunc
		// the natural assumption would be that a1 is C_DOTA_BaseNPC*
		Index OnWearablesChanged{};

		// JS functions
		Index IsRoshan{};
		Index GetIdealSpeed{};
		Index GetAttackRange{};
		Index GetPhysicalArmorValue{};
		Index GetMagicalArmorValue{};
	}
	
	// client.dll
	namespace CDOTABaseAbility {
		// JS functions
		Index GetAbilityTextureName{};
		Index GetEffectiveCastRange{};
		Index GetManaCost{};
	}
	
	// client.dll
	namespace CDOTA_Buff {
		// right above "dota_portrait_unit_modifiers_changed"
		Index OnAddModifier{};
	}

	// networksystem.dll
	namespace CNetChan {
		// calls a function with xref "CTSQueue corruption"
		Index PostReceivedNetMessage{};
		// xref: "CNetChan::SendNetMessage"
		Index SendNetMessage{};
	}
	
	// panorama.dll
	namespace CUIEngineSource2 {
		// xref: "CUIEngine::RunFrame"
		Index RunFrame{};
		// 7th above vfunc with "CUIEngine::RegisterEventHandler"
		// look for the one that matches the dylib version
		Index IsValidPanelPointer{};
		// xref: "CUIEngine::RunScript (compile+run)"
		Index RunScript{};
	}
	
	// client.dll
	namespace CSource2Client {
		// look into the function behind "cl_showents" to find the EntitySystem qword
		// then look at VMs around the broken index in search of a small sub which does mov XXX, g_pEntitySystem
		Index GetNetworkFieldChangeCallbackQueue{};
		// 8 funcs above xrefs "reason_code", "client_disconnect"
		Index VoiceReliable{};
		// xrefs: "userid", "steamid", "player_info"
		Index NotifyDisconnect{};

		// xref: "C:\\buildworker\\source2_dota_rel_2019_win64\\build\\src\\game\\client\\cdll_client_int.cpp"
		Index FrameStageNotify{};
	}
	
	// client.dll
	namespace CRenderGameSystem {
		// Last vfunc of class
		Index WorldToProjectionMatrix{};
	}

	// engine2.dll
	namespace CEngineServiceMgr {
		// CEngineClient vfunc 49 GetScreenSize redirects to this
		// Changes are rarer in this one
		Index GetScreenSize{};
	}

	namespace CNetworkGameClient {
		// 2 funcs below xrefs: "CL:  Loading groups %d\n", "%s:  Entity Group loaded\n"
		Index GetLocalPlayer{};
	}

	namespace CNetworkMessages {
		// directly below xref "Cannot register change callback priorities at this stage!"
		Index FindNetworkMessageByID{};
	}

#define TABLE_ENTRY(x) { #x, &VMI::x },
	static std::map<std::string, uint32_t*> bindings = {
		TABLE_ENTRY(CBaseEntity::GetSchemaBinding)

		TABLE_ENTRY(CSource2Client::FrameStageNotify)
		TABLE_ENTRY(CSource2Client::VoiceReliable)
		TABLE_ENTRY(CSource2Client::GetNetworkFieldChangeCallbackQueue)
		TABLE_ENTRY(CSource2Client::NotifyDisconnect)

		TABLE_ENTRY(CUIEngineSource2::RunFrame)
		TABLE_ENTRY(CUIEngineSource2::IsValidPanelPointer)
		TABLE_ENTRY(CUIEngineSource2::RunScript)

		TABLE_ENTRY(CDOTA_Buff::OnAddModifier)

		TABLE_ENTRY(CDOTABaseNPC::GetAttackRange)
		TABLE_ENTRY(CDOTABaseNPC::GetIdealSpeed)
		TABLE_ENTRY(CDOTABaseNPC::GetPhysicalArmorValue)
		TABLE_ENTRY(CDOTABaseNPC::GetMagicalArmorValue)
		TABLE_ENTRY(CDOTABaseNPC::IsRoshan)
		TABLE_ENTRY(CDOTABaseNPC::OnWearablesChanged)

		TABLE_ENTRY(CDOTABaseAbility::GetManaCost)
		TABLE_ENTRY(CDOTABaseAbility::GetEffectiveCastRange)
		TABLE_ENTRY(CDOTABaseAbility::GetAbilityTextureName)

		TABLE_ENTRY(CNetworkMessages::FindNetworkMessageByID)

		TABLE_ENTRY(CDOTAGameRules::GetGameTime)

		TABLE_ENTRY(CDOTAParticleManager::CreateParticle)

		//TABLE_ENTRY(CEngineClient::IsInGame)

		TABLE_ENTRY(CParticleCollection::SetControlPoint)
		TABLE_ENTRY(CParticleCollection::SetRenderingEnabled)

		TABLE_ENTRY(CRenderGameSystem::WorldToProjectionMatrix)

		TABLE_ENTRY(CEngineServiceMgr::GetScreenSize)

		TABLE_ENTRY(CPanel2D::GetPositionWithinWindow)
		TABLE_ENTRY(CUIPanel::BHasClass)

		TABLE_ENTRY(CNetworkGameClient::GetLocalPlayer)
	};
#undef TABLE_ENTRY
#undef Index
}