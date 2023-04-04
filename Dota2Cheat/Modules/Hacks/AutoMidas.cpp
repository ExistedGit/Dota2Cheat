#include "AutoMidas.h"

void Hacks::AutoMidas::FrameBasedLogic() {
	if (!Config::AutoMidas::Enabled)
		return;

	if (lastTime != 0 &&
		GameSystems::GameRules->GetGameTime() - lastTime < usePeriod)
		return;

	auto midas = ctx.importantItems.midas;

	if (!midas || midas->GetCooldown() != 0)
		return;

	for (auto& creep : ctx.creeps) {

		// If the creep is visible, not one of ours, is alive, is within Midas's radius and its name matches one of the filters
		if (
			creep->GetTeam() != ctx.assignedHero->GetTeam()
			&& creep->GetHealth() > 0
			&& !creep->IsWaitingToSpawn()
			&& !creep->HasState(MODIFIER_STATE_MAGIC_IMMUNE)
			&& IsWithinRadius
			(
				creep->GetPos(),
				ctx.assignedHero->GetPos(),
				midas->GetEffectiveCastRange()
			)
			&& !creep->IsAncient()
			&& !creep->IsRoshan()
			&& creep->Member<int>(Netvars::C_DOTA_BaseNPC::m_iXPBounty) >= Config::AutoMidas::XPTreshold
			) {

			// Don't want to hurt ourselves
			if (strstr(creep->GetUnitName(), "necronomicon_warrior"))
				return;

			ctx.localPlayer->PrepareOrder(
				DOTA_UNIT_ORDER_CAST_TARGET,
				creep->GetIndex(),
				&Vector::Zero,
				midas->GetIndex(),
				DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
				ctx.assignedHero);
		}
	}

	lastTime = GameSystems::GameRules->GetGameTime();
}
