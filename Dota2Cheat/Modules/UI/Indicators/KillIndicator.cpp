#include "KillIndicator.h"

void Modules::M_KillIndicator::Draw() {
	if (!Config::Indicators::Kill || !Initialized)
		return;

	MTM_LOCK;

	float scale = Config::Indicators::KillScale;

	ID3D11ShaderResourceView* icon = nullptr;

	if (nukeIcon) icon = assets.spellIcons.Load(nukeIcon);

	auto dl = ImGui::GetBackgroundDrawList();

	for (const auto& [idx, data] : renderData) {
		if (!data.drawable) continue;

		auto indicatorBase = data.pos - ImVec2(76, 6);
		auto plaqueXY = indicatorBase - ImVec2(26, 0) * scale;

		if (data.diff <= 0)
			// Outline
			dl->AddRectFilled(indicatorBase, indicatorBase + ImVec2(18, 18) * scale, ImColor(0, 230, 0, 255), (3 * scale));
		else {
			auto diffStr = std::to_string(std::clamp(data.diff, 0, 999));

			auto textBase = plaqueXY + ImVec2(24 * scale / 2, (18 - 12) * scale / 2);

			// Outline
			dl->AddRectFilled(indicatorBase, indicatorBase + ImVec2(18, 18) * scale, ImColor(255, 50, 50, 255), (3 * scale));

			// Background & text
			dl->AddRectFilled(plaqueXY, plaqueXY + ImVec2(24, 18) * scale, ImColor(0, 0, 0, 255), 2 * scale);
			ImDrawText(DrawData.GetFont("Monofonto", scale * 16), diffStr, textBase, scale * 12, ImVec4(1, 1, 1, 1), true, false);
		}

		// Icon
		dl->AddRectFilled(indicatorBase + ImVec2{ 2,2 }, indicatorBase + ImVec2(16, 16) * scale, ImColor(1, 1, 1, 255), (2 * scale));
		dl->AddImageRounded(icon, indicatorBase + ImVec2{ 3,3 }, indicatorBase + ImVec2(15, 15) * scale, { 0, 0 }, { 1, 1 }, ImColor(255, 255, 255, 255), (2 * scale));
	}
}

void Modules::M_KillIndicator::OnFrame()
{
	if (!Config::Indicators::Kill || !Initialized) return;

	MTM_LOCK;

	auto nuke = ctx.localHero->GetAbility(curData.idx);
	if (nuke->GetLevel() == 0)
		return;

	nukeIcon = nuke->GetAbilityTextureName();

	for (auto& [idx, data] : renderData) {
		auto hero = CEntSys::Get()->GetEntity<CHero>(idx);

		data.drawable =
			!hero->IsSameTeam(ctx.localHero)
			&& hero->IsTargetable()
			&& !hero->IsIllusion()
			&& IsPointOnScreen(HeroData[hero].W2S);

		if (!data.drawable) continue;

		data.diff = CalcDmgWithResists(hero, 
			CustomBehaviors.contains(ctx.localHero->GetUnitName())
			? CustomBehaviors.at(ctx.localHero->GetUnitName())(nuke, hero)
			: CalculationBehaviors::Default(nuke, hero), curData.isPure);

		data.pos = HeroData[hero].HealthbarW2S;
	}
}

void Modules::M_KillIndicator::OnEntityAdded(const EntityWrapper& ent)
{
	if (ent.type != EntityType::Hero) return;
	renderData.insert({ ent->GetIndex(),RenderData() });
}

void Modules::M_KillIndicator::OnEntityRemoved(const EntityWrapper& ent)
{
	renderData.erase(ent->GetIndex());
}
