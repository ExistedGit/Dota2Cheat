#include "TrueSightESP.h"

void Modules::M_TrueSightESP::OnModifierRemoved(CDOTAModifier* modifier) {
	if (!TrackedModifiers.count(modifier))
		return;
	GameSystems::ParticleManager->DestroyParticle(TrackedModifiers[modifier]);
	TrackedModifiers.erase(modifier);
}

void Modules::M_TrueSightESP::OnModifierAdded(CDOTAModifier* modifier) {
	if (!Config::ModifierRevealer::TrueSight)
		return;

	auto owner = modifier->GetOwner();
	if (!owner->IsSameTeam(ctx.localHero)
		||
		!EntityList.IsEntityOfType(owner, EntityType::Hero) &&
		!strstr(owner->SchemaBinding()->binaryName, "Observer_Ward")) // yes, you can tell if a ward is under a sentry 
		return;

	if (strcmp(modifier->GetName(), "modifier_truesight") != 0)
		return;

	TrackedModifiers[modifier] = GameSystems::ParticleManager->CreateParticle(
		"particles/items2_fx/ward_true_sight_true_sight.vpcf",
		PATTACH_OVERHEAD_FOLLOW,
		modifier->GetOwner()
	);
	TrackedModifiers[modifier].particle->SetControlPoint(0, { 0,0, 0 });
}
