#include "IllusionColoring.h"

void Modules::M_IllusionESP::ColorIfIllusion(CBaseEntity* ent) {
	if (!Config::IllusionColoring::Enabled || frameDone)
		return;

	auto hero = (CDOTABaseNPC_Hero*)ent;
	if (!EntityList.IsEntityOfType(hero, EntityType::Hero))
		return;

	const char* className = ent->SchemaBinding()->binaryName;
	auto& isSeenAsIllusion = ent->Field<bool>(Netvars::C_DOTA_BaseNPC::m_bHasClientSeenIllusionModifier);

	if (ctx.localHero &&
		!ctx.localHero->IsSameTeam(hero) &&
		!isSeenAsIllusion &&
		hero->IsIllusion()
		) {

		//const auto& m_clrRender = Config::IllusionColoring::Color;
		//hero->SetColor(Color(m_clrRender.x * 255, m_clrRender.y * 255, m_clrRender.z * 255));

		// from CDOTA_Modifier_Illusion::OnCreated
		// also observed via cl_particle_log_creates
		auto p = GameSystems::ParticleManager->CreateParticle(
			"particles/status_fx/status_effect_illusion.vpcf",
			PATTACH_ABSORIGIN_FOLLOW,
			ent
		);

		static Function GetModifierFactoryDictionary = Memory::Scan("E8 ? ? ? ? 48 85 C0 8B C5", "client.dll")
			.GetAbsoluteAddress(1)
			.Offset(0x18)
			.GetAbsoluteAddress(1);

		static auto dict = GetModifierFactoryDictionary.Call<VClass*>();

		auto modifier = dict->CallVFunc<1, CDOTAModifier*>("modifier_illusion");

		modifier->Field<const char*>(Netvars::CDOTA_Buff::m_name) = "modifier_illusion";
		modifier->Field<void*>(0x30) = hero->GetModifierManager();
		modifier->Particles().push_back({
					.particleHandle = p.handle,
					.entIdx = ent->GetIndex()
			});

		auto& list = hero->GetModifierManager()->GetModifierList();
		list.push_back(modifier);

		isSeenAsIllusion = true;
		frameDone = true;
		return;
	}

}
