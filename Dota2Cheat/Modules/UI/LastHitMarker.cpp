#include "LastHitMarker.h"

void Modules::M_LastHitMarker::Draw() {
	if (!Config::LastHitMarker)
		return;

	for (const auto& [idx, data] : renderData) {
		if (!data.drawable) continue;

		ImColor color = 
			data.allied
			? ImColor{ 0,255,0 } 
			: ImColor{ 255,0,0 };
		float radius = 10 * (1200 / Config::CameraDistance * 1.2);
		ImGui::GetBackgroundDrawList()->AddCircleFilled(data.pos, radius, color);
	}
}

void Modules::M_LastHitMarker::OnFrame()
{

	if (!Config::LastHitMarker)
		return;

	bool hasQBlade =
		HeroData[ctx.localHero].Modifiers.contains("modifier_item_quelling_blade") ||
		HeroData[ctx.localHero].Modifiers.contains("modifier_item_battlefury");

	const float attackRange = ctx.localHero->GetAttackRange();

	for (auto& [idx, data] : renderData) {
		data.drawable = false;

		auto creep = CEntSys::Get()->GetEntity<CNPC>(idx);
		if (!creep
			|| !creep->GetIdentity()
			|| !creep->IsTargetable())
			continue;

		// Distance check
		if (!IsWithinRadius(
			creep->GetPos(),
			ctx.localHero->GetPos(),
			ctx.localHero->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK ? 1000 : attackRange * 1.2
		))
			continue;

		// Deny check
		if (creep->IsSameTeam(ctx.localHero) && (float)creep->GetHealth() / creep->GetMaxHealth() >= 0.5f)
			continue;

		int dmg = ctx.localHero->GetAttackDamageMin();

		if (hasQBlade)
			dmg += ctx.localHero->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK ? 8 : 4;

		if (data.IsSiege())
			dmg *= 0.5f;

		float dmgReduction = (0.06f * creep->GetPhysicalArmorValue()) / (1 + 0.06f * abs(creep->GetPhysicalArmorValue()));
		// Damage check
		if (creep->GetHealth() >= dmg * (1 - dmgReduction))
			continue;

		data.pos = WorldToScreen(creep->GetHealthBarPos());
		data.allied = creep->IsSameTeam(ctx.localHero);

		data.drawable = true;
	}
}
