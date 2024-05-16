#include "IllusionColoring.h"
#include <dota_modifiers.pb.h>

void Modules::M_IllusionESP::AddIllusionModifier(CDOTABaseNPC_Hero* hero, const char* modifierName, const char* particle) {
	//auto p = GameSystems::ParticleManager->CreateParticle(
	//	particle,
	//	PATTACH_ABSORIGIN_FOLLOW,
	//	hero
	//);

	// C_DOTA_BaseNPC::ClientAddNewModifier
	// xref: "dota_portrait_unit_modifiers_changed" and go up the xref ladder
	using fC_DOTA_BaseNPC__ClientAddNewModifier = void(*)(CDOTABaseNPC* thisptr, CBaseEntity* caster, CDOTABaseAbility* ability, const char* name, KeyValues* kv, int unk, CDOTAModifierBuffTableEntry* entry);
	static fC_DOTA_BaseNPC__ClientAddNewModifier C_DOTA_BaseNPC__ClientAddNewModifier = Memory::Scan("E8 ? ? ? ? 48 85 C0 74 06 8B 4D EC", "client.dll").GetAbsoluteAddress(1);
	static auto buffKV = KeyValues::MakeKV("illusion_kv");
	
	buffKV->SetString("BaseClass", modifierName);
	
	CDOTAModifierBuffTableEntry entry;
	entry.set_serial_num(0);
	entry.set_parent(hero->GetIndex());
	entry.set_index(300000000);

	C_DOTA_BaseNPC__ClientAddNewModifier(hero, nullptr, nullptr, modifierName, buffKV, 0, &entry);
}

void Modules::M_IllusionESP::ColorIllusion(CDOTABaseNPC_Hero* hero) {
	if (ctx.localHero->IsSameTeam(hero) ||
		!hero->IsIllusion())
		return;

	auto& isSeenAsIllusion = hero->Field<bool>(Netvars::C_DOTA_BaseNPC::m_bHasClientSeenIllusionModifier);

	const auto& m_clrRender = Config::IllusionColoring::Color;
	hero->SetColor(Color(m_clrRender.x * 255, m_clrRender.y * 255, m_clrRender.z * 255));
	if (!isSeenAsIllusion) {
		// from CDOTA_Modifier_Illusion::OnCreated
		// also observed via cl_particle_log_creates
		AddIllusionModifier(hero, "modifier_illusion", "particles/status_fx/status_effect_illusion.vpcf");
		isSeenAsIllusion = true;
	}
}
