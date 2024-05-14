#include "TrueSightESP.h"

void Modules::M_TrueSightESP::OnModifierRemoved(CDOTAModifier* modifier) {
	if (!TrackedModifiers.contains(modifier))
		return;
	GameSystems::ParticleManager->DestroyParticle(TrackedModifiers[modifier]);
	TrackedModifiers.erase(modifier);
}

void Modules::M_TrueSightESP::OnModifierAdded(CDOTAModifier* modifier) {
	if (!ctx.localHero || !Config::ModifierRevealer::TrueSight)
		return;

	auto owner = modifier->GetOwner();
	if (!owner->IsSameTeam(ctx.localHero) ||
		!EntityList.IsEntityOfType(owner, EntityType::Hero) &&
		owner->SchemaBinding()->binaryName != "Observer_Ward"sv) // yes, you can tell if a ward is under a sentry
		return;

	if (modifier->GetName() != "modifier_truesight"sv)
		return;

	TrackedModifiers[modifier] = GameSystems::ParticleManager->CreateParticle(
		"particles/items2_fx/ward_true_sight_true_sight.vpcf",
		PATTACH_OVERHEAD_FOLLOW,
		modifier->GetOwner()
	);
	TrackedModifiers[modifier].particle->SetControlPoint(0, { 0,0, 0 });
}
