#pragma once
#include <json.hpp>
#include <map>
#include <string>

class VMDB {
	inline static nlohmann::json Data;
public:
	static nlohmann::json* GetNestedJsonEntry(const std::string& name);

	// Use a map of VM names to index pointers to fill in their indexes
	static void ParseMap(const std::map <std::string, uint32_t*>& m);

	static void LoadFromFile(const std::string& url);;
};


namespace VTableIndexes {
#define Index inline uint32_t

	namespace CEngineClient {
		// after GetMaxClients(which returns 64/0x40)
		// either bruteforce or compare which vfunc of xmmword_XXXXXXXX the function called last time and compare with new data
		Index IsInGame{};
	}
	namespace CParticleCollection {
		// xref: "modifier_item_ward_true_sight", appears twice in the same func
		// decompile -> under "particles/ui_mouseactions/range_display.vpcf" will be a sub_XXXXXXX call with 3 args
		// that is CNewParticleEffect::SetControlPoint, which gets the CParticleCollection at 0x20 and calls the vfunc at index 16
		Index SetControlPoint{};
		// xref: "Error in child list of particle system %s [%p]"
		// easy to see
		Index SetRenderingEnabled{};
	}
	namespace CDOTAParticleManager {
		// JS xref "CreateParticle" to lea RCX
		// Second call is GetParticleManager
		// Third calls a virtual function of RAX, the index is there
		Index CreateParticle{};
	}
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
	namespace CDOTABaseAbility {
		// JS
		Index GetEffectiveCastRange{};
	}
	namespace CDOTA_Buff {
		Index OnAddModifier{};
	}
	namespace CUIEngineSource2 {
		// xref: "CUIEngine::RunFrame"
		Index RunFrame{};
		// 7th above vfunc with "CUIEngine::RegisterEventHandler"
		// look for the one that matches the dylib version
		Index IsValidPanelPointer{};
		// xref: "CUIEngine::RunScript (compile+run)"
		Index RunScript{};
	}
	namespace CSource2Client {
		// look into the function behind "cl_showents" to find the EntitySystem qword
		// then look at VMs around the broken index in search of a small sub which does mov XXX, g_pEntitySystem
		Index GetNetworkFieldChangeCallbackQueue{};
		// 8 funcs above xrefs "reason_code", "client_disconnect"
		Index VoiceReliable{};
		// xrefs: "userid", "steamid", "player_info"
		Index NotifyDisconnect{};
	}

#define TABLE_ENTRY(x) { #x, &x },
	std::map<std::string, uint32_t*> vmTables = {
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

		TABLE_ENTRY(CDOTABaseAbility::GetEffectiveCastRange)

		TABLE_ENTRY(CDOTAParticleManager::CreateParticle)

		TABLE_ENTRY(CEngineClient::IsInGame)
		
		TABLE_ENTRY(CParticleCollection::SetControlPoint)
		TABLE_ENTRY(CParticleCollection::SetRenderingEnabled)
	};

#undef Index

	inline void Load(const std::string& file) {
		VMDB::LoadFromFile(file);
		VMDB::ParseMap(vmTables);
	}
}