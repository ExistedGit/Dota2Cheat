#include "AutoMidas.h"

void Hacks::AutoMidas::FrameBasedLogic() {
	if (!Config::AutoMidas::Enabled)
		return;

	//if (lastPickupTime != 0 &&
	//	GameSystems::GameRules->GetGameTime() - lastPickupTime < usePeriod)
	//	return;

	auto midas = HeroData[ctx.localHero].Items["hand_of_midas"];

	if (!midas || midas->GetCooldown() != 0)
		return;

	for (auto& wrapper : ctx.creeps) {
		auto creep = wrapper.ent;
		// If the creep is visible, not one of ours, is alive, is within Midas's radius and its name matches one of the filters
		if (creep->IsSameTeam(ctx.localHero))
			continue;

		if (!creep->IsTargetable())
			continue;

		if (creep->IsAncient() ||
			creep->IsRoshan())
			continue;

		if (creep->Member<int>(Netvars::C_DOTA_BaseNPC::m_iXPBounty) < Config::AutoMidas::XPTreshold)
			continue;

		if (creep->HasState(MODIFIER_STATE_MAGIC_IMMUNE))
			continue;
		if (!IsWithinRadius
		(
			creep->GetPos(),
			ctx.localHero->GetPos(),
			midas->GetEffectiveCastRange()
		)
			)
			continue;

		// Don't want to hurt ourselves
		if (strstr(creep->GetUnitName(), "necronomicon_warrior"))
			continue;

		ctx.localPlayer->PrepareOrder(
			DOTA_UNIT_ORDER_CAST_TARGET,
			creep->GetIndex(),
			&Vector::Zero,
			midas->GetIndex(),
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			ctx.localHero);

		lastTime = GameSystems::GameRules->GetGameTime();
	}
}
