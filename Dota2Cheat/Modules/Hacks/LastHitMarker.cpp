#include "LastHitMarker.h"

void Hacks::LastHitMarker::DrawCircleFor(CDOTABaseNPC* creep) {
	ImColor color = creep->IsSameTeam(ctx.assignedHero) ?
		ImColor{ 0,255,0 } :
		ImColor{ 255,0,0 };
	auto pos = creep->GetPos();
	pos.z += creep->Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);
	float radius = 10 * (1200 / Config::CameraDistance * 1.2);
	ImGui::GetForegroundDrawList()->AddCircleFilled(WorldToScreen(pos), radius, color);
}

void Hacks::LastHitMarker::Draw() {
	if (!Config::LastHitMarker)
		return;

	for (auto& wrapper : ctx.creeps) {
		auto creep = wrapper.ent;
		if (!IsValidReadPtr(creep)
			|| !IsValidReadPtr(creep->GetIdentity())
			|| !creep->IsTargetable())
			continue;

		// Distance check
		if (!IsWithinRadius(creep->GetPos(), ctx.assignedHero->GetPos(), 1000))
			continue;

		// Deny check
		if (creep->IsSameTeam(ctx.assignedHero) && (float)creep->GetHealth() / creep->GetMaxHealth() >= 0.5f)
			continue;

		int dmg = ctx.assignedHero->GetAttackDamageMin();

		if (ctx.assignedHero->HasOneOfModifiers({
			"modifier_item_quelling_blade",
			"modifier_item_battlefury"
			}))
			dmg += ctx.assignedHero->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK ? 8 : 4;

		if (wrapper.creepType == CreepType::Siege)
			dmg *= 0.5f;

		float dmgReduction = (0.052f * creep->GetPhysicalArmorValue()) / (0.9f + 0.048f * abs(creep->GetPhysicalArmorValue()));
		// Damage check
		if (creep->GetHealth() >= dmg * (1 - dmgReduction))
			continue;

		DrawCircleFor(creep);
	}
}
