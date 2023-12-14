#include "IllusionColoring.h"

void Modules::M_IllusionESP::AddIllusionModifier(CDOTABaseNPC_Hero* hero, const char* modifierName, const char* particle) {
	auto p = GameSystems::ParticleManager->CreateParticle(
		particle,
		PATTACH_ABSORIGIN_FOLLOW,
		hero
	);

	// ModifierFactoryDictionary
	static auto dict = Memory::Scan("48 83 EC 20 E8 ? ? ? ? 8B 0D", "client.dll")
		.Offset(4)
		.GetAbsoluteAddress(1)
		.Offset(0x27)
		.GetAbsoluteAddress<VClass*>(3);

	// Creates the modifier and puts our particle into its m_iParticles
	auto modifier = dict->CallVFunc<1, CDOTAModifier*>(modifierName);

	modifier->Field<const char*>(Netvars::CDOTA_Buff::m_name) = modifierName;
	modifier->Field<void*>(0x30) = hero->GetModifierManager();
	modifier->Particles().push_back({
		.particleHandle = p.handle,
		.entIdx = hero->GetIndex()
		});

	hero->GetModifierManager()->GetModifierList().push_back(modifier);
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
