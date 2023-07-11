#include "EnemySpellHighlight.h"

void Modules::EnemySpellHighlighter::DrawParticleAt(Vector pos, ParticleCreationInfo info) {
	auto particleWrap = GameSystems::ParticleManager->CreateParticle(
		info.particleName,
		PATTACH_WORLDORIGIN,
		nullptr);
	particleWrap.particle
		->SetControlPoint(0, pos);
	for (auto& [idx, val] : info.controlPoints)
		particleWrap.particle->SetControlPoint(idx, val);

	if (info.dieTime)
		Modules::ParticleGC.SetDieTime(particleWrap, info.dieTime);
}

void Modules::EnemySpellHighlighter::OnModifierAdded(CDOTAModifier* modifier) {
	if (!Config::ShowEnemyPointSpells)
		return;

	if (ModifierParticles.count(modifier->GetName())) {
		auto thinker = modifier->GetOwner();
		if (!thinker->IsSameTeam(ctx.localHero)) {

			DrawParticleAt(thinker->GetPos(), ModifierParticles[modifier->GetName()]);
		}
	}
}
