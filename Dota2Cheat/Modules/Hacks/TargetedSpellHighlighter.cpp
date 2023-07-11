#include "TargetedSpellHighlighter.h"

void Modules::M_TargetedSpellHighlighter::Reset() {
	OnDisableTargetedSpells();
	OnDisableLinken();
	HeroesWithLinken.clear();
}

void Modules::M_TargetedSpellHighlighter::OnDisableTargetedSpells() {
	for (auto& [_, pw] : TrackedModifiers)
		if (HVALID(pw.handle))
			GameSystems::ParticleManager->DestroyParticle(pw);
	TrackedModifiers.clear();
}

void Modules::M_TargetedSpellHighlighter::OnDisableLinken() {
	for (auto& [hero, _] : HeroesWithLinken)
		GameSystems::ParticleManager->DestroyParticle(LinkenSphereParticles[hero]);

	LinkenSphereParticles.clear();
}

void Modules::M_TargetedSpellHighlighter::SubscribeLinkenRendering(CBaseEntity* ent, CDOTABaseAbility* sphere) {
	HeroesWithLinken[ent] = sphere;
}

void Modules::M_TargetedSpellHighlighter::UnsubscribeLinkenRendering(CBaseEntity* ent) {
	RemoveLinkenEffectFor(ent);
	HeroesWithLinken.erase(ent);
}

void Modules::M_TargetedSpellHighlighter::RemoveLinkenEffectFor(CBaseEntity* ent) {
	if (!LinkenSphereParticles.count(ent))
		return;

	GameSystems::ParticleManager->DestroyParticle(LinkenSphereParticles[ent]);
	LinkenSphereParticles.erase(ent);
}

void Modules::M_TargetedSpellHighlighter::DrawLinkenEffectFor(CBaseEntity* ent) {
	if (!Config::ModifierRevealer::LinkenSphere)
		return;

	if (LinkenSphereParticles.count(ent))
		return;

	LinkenSphereParticles[ent] = GameSystems::ParticleManager->CreateParticle(
		"particles/items_fx/immunity_sphere_buff.vpcf",
		PATTACH_ROOTBONE_FOLLOW,
		ent
	);

}

void Modules::M_TargetedSpellHighlighter::OnFrame() {

	for (auto& [hero, ability] : HeroesWithLinken) {
		if (!hero->GetIdentity()->IsDormant() && ability->GetCooldown() == 0)
			DrawLinkenEffectFor(hero);
		else
			RemoveLinkenEffectFor(hero);
	}
}

void Modules::M_TargetedSpellHighlighter::OnModifierRemoved(CDOTAModifier* modifier) {
	if (!TrackedModifiers.count(modifier))
		return;
	GameSystems::ParticleManager->DestroyParticle(TrackedModifiers[modifier]);
	TrackedModifiers.erase(modifier);
}

void Modules::M_TargetedSpellHighlighter::OnModifierAdded(CDOTAModifier* modifier) {
	if (!Config::ModifierRevealer::TargetedSpells)
		return;
	auto buffName = modifier->GetName();
	if (!buffName || !ModifierParticles.count(buffName))
		return;

	if (modifier->IsSameTeam(ctx.localHero))
		return;

	auto entry = ModifierParticles[buffName];
	if (AdditionalChecks.contains(buffName) && !AdditionalChecks[buffName](modifier))
		return;

	TrackedModifiers[modifier] = GameSystems::ParticleManager->CreateParticle(
		entry.particleName,
		PATTACH_OVERHEAD_FOLLOW,
		modifier->GetOwner()
	);
	for (auto& [idx, value] : entry.controlPoints)
		TrackedModifiers[modifier].particle
		->SetControlPoint(idx, value);

	if (entry.dieTime)
		Modules::ParticleGC.SetDieTime(TrackedModifiers[modifier], entry.dieTime);

}
