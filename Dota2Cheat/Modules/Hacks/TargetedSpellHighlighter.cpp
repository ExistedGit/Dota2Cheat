#include "TargetedSpellHighlighter.h"

void Hacks::TargetedSpellHighlighter::Reset() {
	OnDisableTargetedSpells();
	OnDisableLinken();
	HeroesWithLinken.clear();
}

void Hacks::TargetedSpellHighlighter::OnDisableTargetedSpells() {
	for (auto& [_, pw] : TrackedModifiers)
		if (pw.handle != 0xFFFFFFFF)
			GameSystems::ParticleManager->DestroyParticle(pw);
	TrackedModifiers.clear();
}

void Hacks::TargetedSpellHighlighter::OnDisableLinken() {
	for (auto& [hero, _] : HeroesWithLinken)
		GameSystems::ParticleManager->DestroyParticle(LinkenSphereParticles[hero]);

	LinkenSphereParticles.clear();
}

void Hacks::TargetedSpellHighlighter::SubscribeLinkenRendering(CBaseEntity* ent, CDOTABaseAbility* sphere) {
	HeroesWithLinken[ent] = sphere;
}

void Hacks::TargetedSpellHighlighter::UnsubscribeLinkenRendering(CBaseEntity* ent) {
	RemoveLinkenEffectFor(ent);
	HeroesWithLinken.erase(ent);
}

void Hacks::TargetedSpellHighlighter::RemoveLinkenEffectFor(CBaseEntity* ent) {
	if (!LinkenSphereParticles.count(ent))
		return;

	GameSystems::ParticleManager->DestroyParticle(LinkenSphereParticles[ent]);
	LinkenSphereParticles.erase(ent);
}

void Hacks::TargetedSpellHighlighter::DrawLinkenEffectFor(CBaseEntity* ent) {
	if (!Config::ShowLinkenSphere)
		return;

	if (LinkenSphereParticles.count(ent))
		return;

	LinkenSphereParticles[ent] = GameSystems::ParticleManager->CreateParticle(
		"particles/items_fx/immunity_sphere_buff.vpcf",
		PATTACH_ROOTBONE_FOLLOW,
		ent
	);

}

void Hacks::TargetedSpellHighlighter::FrameBasedLogic() {

	for (auto& [hero, ability] : HeroesWithLinken) {
		if (!hero->GetIdentity()->IsDormant() && ability->GetCooldown() == 0)
			DrawLinkenEffectFor(hero);
		else
			RemoveLinkenEffectFor(hero);
	}
}

void Hacks::TargetedSpellHighlighter::RemoveParticleIfTargetedSpell(CDOTAModifier* modifier) {
	if (!TrackedModifiers.count(modifier))
		return;
	GameSystems::ParticleManager->DestroyParticle(TrackedModifiers[modifier]);
	TrackedModifiers.erase(modifier);
}

void Hacks::TargetedSpellHighlighter::DrawParticleIfTargetedSpell(CDOTAModifier* modifier) {
	if (!Config::ShowEnemyTargetedSpells)
		return;
	if (!ModifierParticles.count(modifier->GetName()))
		return;
	if (modifier->GetOwner()->GetTeam() != ctx.assignedHero->GetTeam())
		return;

	auto entry = ModifierParticles[modifier->GetName()];

	TrackedModifiers[modifier] = GameSystems::ParticleManager->CreateParticle(
		entry.particleName,
		PATTACH_OVERHEAD_FOLLOW,
		modifier->GetOwner()
	);
	for (auto& cp : entry.controlPoints)
		TrackedModifiers[modifier].particle
		->SetControlPoint(cp.first, &cp.second);

	if (entry.dieTime)
		Modules::ParticleGC.SetDieTime(TrackedModifiers[modifier], entry.dieTime);

}
