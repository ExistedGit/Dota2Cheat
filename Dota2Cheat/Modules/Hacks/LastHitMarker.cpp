#include "LastHitMarker.h"

void Modules::M_LastHitMarker::DrawCircleFor(CDOTABaseNPC* creep) {
	ImColor color = creep->IsSameTeam(ctx.localHero) ?
		ImColor{ 0,255,0 } :
		ImColor{ 255,0,0 };
	float radius = 10 * (1200 / Config::CameraDistance * 1.2);
	ImGui::GetForegroundDrawList()->AddCircleFilled(WorldToScreen(creep->GetHealthBarPos()), radius, color);
}

void Modules::M_LastHitMarker::Draw() {
	if (!Config::LastHitMarker)
		return;

	bool hasQBlade =
		HeroData[ctx.localHero].Modifiers.contains("modifier_item_quelling_blade") ||
		HeroData[ctx.localHero].Modifiers.contains("modifier_item_battlefury");

	auto attackRange = ctx.localHero->GetAttackRange();
	const auto DrawForCreep = [this, hasQBlade, attackRange](const auto& wrapper) {

		auto creep = wrapper.As<CDOTABaseNPC>();
		if (!IsValidReadPtr(creep)
			|| !IsValidReadPtr(creep->GetIdentity())
			|| !creep->IsTargetable())
			return;

		// Distance check
		if (!IsWithinRadius(
			creep->GetPos(),
			ctx.localHero->GetPos(),
			ctx.localHero->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK ? 1000 : attackRange * 1.2
		))
			return;

		// Deny check
		if (creep->IsSameTeam(ctx.localHero) && (float)creep->GetHealth() / creep->GetMaxHealth() >= 0.5f)
			return;

		int dmg = ctx.localHero->GetAttackDamageMin();

		if (hasQBlade)
			dmg += ctx.localHero->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK ? 8 : 4;

		if (wrapper.creepType == CreepType::Siege)
			dmg *= 0.5f;

		float dmgReduction = (0.06f * creep->GetPhysicalArmorValue()) / (1 + 0.06f * abs(creep->GetPhysicalArmorValue()));
		// Damage check
		if (creep->GetHealth() >= dmg * (1 - dmgReduction))
			return;

		DrawCircleFor(creep);
	};
	EntityList.ForEachOfType(EntityType::Creep, DrawForCreep);
}
