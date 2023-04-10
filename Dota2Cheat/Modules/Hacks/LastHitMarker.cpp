#include "LastHitMarker.h"

void Hacks::LastHitMarker::DrawCircleFor(CDOTABaseNPC* creep) {
	ImColor color = creep->GetTeam() == ctx.assignedHero->GetTeam() ?
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

	for (auto& creep : ctx.creeps) {
		if (!IsValidReadPtr(creep)
			|| !IsValidReadPtr(creep->GetIdentity())
			|| !creep->IsTargetable())
			continue;

		// Distance check
		if (!IsWithinRadius(creep->GetPos(), ctx.assignedHero->GetPos(), 1000))
			continue;

		// Deny check
		if (creep->GetTeam() == ctx.assignedHero->GetTeam() && creep->GetHealth() / creep->GetMaxHealth() >= 0.5f)
			continue;

		// Damage check
		if (creep->GetHealth() >= ctx.assignedHero->GetAttackDamageMin() * (
			1 - (0.052f* creep->GetPhysicalArmorValue())/(0.9f + 0.048f * abs(creep->GetPhysicalArmorValue()))
			))
			continue;

		DrawCircleFor(creep);
	}
}
