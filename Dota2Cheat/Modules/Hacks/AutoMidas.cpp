#include "AutoMidas.h"

void Modules::M_AutoMidas::OnFrame() {
	if (!Config::AutoMidas::Enabled)
		return;

	//if (lastPickupTime != 0 &&
	//	GameSystems::GameRules->GetGameTime() - lastPickupTime < usePeriod)
	//	return;

	auto midas = HeroData[ctx.localHero].Items["hand_of_midas"];

	if (!midas || midas->GetCooldown() != 0)
		return;

	const auto canMidas = [this, midas](auto& wrap) -> bool {
		auto creep = wrap.As<CDOTABaseNPC>();
		// If the creep is visible, not one of ours, is alive, is within Midas's radius and its name matches one of the filters
		if (creep->IsSameTeam(ctx.localHero))
			return false;

		if (!creep->IsTargetable())
			return false;

		if (creep->IsAncient() ||
			creep->IsRoshan())
			return false;

		if (creep->Member<int>(Netvars::C_DOTA_BaseNPC::m_iXPBounty) < Config::AutoMidas::XPTreshold)
			return false;

		if (creep->HasState(MODIFIER_STATE_MAGIC_IMMUNE))
			return false;
		if (!IsWithinRadius
		(
			creep->GetPos(),
			ctx.localHero->GetPos(),
			midas->GetEffectiveCastRange()
		)
			)
			return false;

		// Don't want to hurt ourselves
		if (strstr(creep->GetUnitName(), "necronomicon_warrior"))
			return false;

		return true;
	};

	EntityList.ForEachOfType(EntityType::Creep, [this, midas, canMidas](auto& wrap) {
		if (canMidas(wrap))
		{
			ctx.localPlayer->PrepareOrder(
				DOTA_UNIT_ORDER_CAST_TARGET,
				wrap->GetIndex(),
				Vector::Zero,
				midas->GetIndex(),
				DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
				ctx.localHero);

			lastTime = GameSystems::GameRules->GetGameTime();
		}
		});
}
