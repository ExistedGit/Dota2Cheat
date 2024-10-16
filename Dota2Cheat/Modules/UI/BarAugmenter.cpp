#include "BarAugmenter.h"

inline bool Modules::M_BarAugmenter::CanDraw(CHero* hero) const {
	return !hero->IsDormant()
		&& !hero->IsSameTeam(ctx.localHero)
		&& !hero->IsIllusion()
		&& hero != ctx.localHero
		&& hero->IsAlive()
		&& IsEntityOnScreen(hero);
}

void Modules::M_BarAugmenter::OnFrame()
{
	if(!Config::Bars::HPNumbers)

	for (auto& [idx, data] : renderData) {
		auto npc = CEntSys::Get()->GetEntity<CHero>(idx);
		data.drawable = CanDraw(npc);

		if (!data.drawable) continue;
		data.health = npc->GetHealth();
		data.pos = WorldToScreen(npc->GetPos());
	}
}

void Modules::M_BarAugmenter::OnEntityAdded(const EntityWrapper& ent)
{
	if (ent.type != EntityType::Hero) return;

	renderData.insert({ ent->GetIndex(), RenderData() });
}

void Modules::M_BarAugmenter::OnEntityRemoved(const EntityWrapper& ent)
{
	renderData.erase(ent->GetIndex());
}

