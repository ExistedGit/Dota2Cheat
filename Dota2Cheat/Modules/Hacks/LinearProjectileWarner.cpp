#include "LinearProjectileWarner.h"


// Draws a dashed red line from begin to end
// Returns the wrapper for the created particle

ParticleWrapper Modules::M_LinearProjectileWarner::DrawTrajectory(const Vector& begin, const Vector& end) {
	auto pw = GameSystems::ParticleManager->CreateParticle(
		"particles/ui_mouseactions/range_finder_tower_line.vpcf",
		PATTACH_WORLDORIGIN,
		nullptr
	);
	pw.particle
		->SetControlPoint(2, begin)
		->SetControlPoint(6, {1,0,0})
		->SetControlPoint(7, end);
	return pw;
}

void Modules::M_LinearProjectileWarner::OnModifierAdded(CDOTAModifier* modifier) {
	if (!Config::ShowLinearProjTrajectory)
		return;

	auto owner = modifier->GetOwner();
	if (AbilityTrajectories.count(modifier->GetName()) &&
		!EntityTrajectories.count(modifier)) {
		auto trajectoryInfo = AbilityTrajectories[modifier->GetName()];
		auto ability = owner->GetAbility(trajectoryInfo.value);
		if (!ability)
			return;

		int offset = 0;
		offset = trajectoryInfo.isAbilitySlot
			? ability->GetEffectiveCastRange()
			: trajectoryInfo.value;

		EntityTrajectories[modifier] = EntTrajectoryInfo{
			.offset = offset,
			.particleWrap = DrawTrajectory(
				owner->GetPos(),
				owner->GetForwardVector(offset))
		};
	}
}

void Modules::M_LinearProjectileWarner::OnModifierRemoved(CDOTAModifier* modifier) {
	if (EntityTrajectories.count(modifier)) {
		GameSystems::ParticleManager->DestroyParticle(EntityTrajectories[modifier].particleWrap);
		EntityTrajectories.erase(modifier);
	}
}

void Modules::M_LinearProjectileWarner::OnFrame() {
	for (auto& [modifier, info] : EntityTrajectories) {
		auto owner = modifier->GetOwner();
		auto forwardVec = modifier->GetOwner()->GetForwardVector(info.offset);
		info.particleWrap.particle->SetControlPoint(7, forwardVec);
	}
}

void Modules::M_LinearProjectileWarner::OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID == 471) {
		auto linProjMsg = reinterpret_cast<CDOTAUserMsg_CreateLinearProjectile*>(msg);
		auto& newProj =
			linearProjectiles[linProjMsg->handle()] =
			LinearProjectile{
			.source = Interfaces::EntitySystem->GetEntity(linProjMsg->entindex()),
			.handle = linProjMsg->handle(),
			.distance = linProjMsg->distance(),
			.velocity = Vector2D(linProjMsg->velocity().x(), linProjMsg->velocity().y()),
			.acceleration = Vector2D(linProjMsg->acceleration().x(), linProjMsg->acceleration().y()),
			.origin = Vector(linProjMsg->origin().x(), linProjMsg->origin().y(), linProjMsg->origin().z())
		};

		if (!Config::ShowLinearProjTrajectory)
			return;
		if (!newProj.source || newProj.source->IsSameTeam(ctx.localHero))
			return;

		auto ratio = newProj.distance / newProj.velocity.Length();
		auto endPoint = newProj.origin;
		endPoint.x += (newProj.velocity * ratio).x;
		endPoint.y += (newProj.velocity * ratio).y;
		TrackedProjectiles[newProj.handle] = DrawTrajectory(newProj.origin, endPoint);

	} else if (msgHandle->messageID == 472) {
		auto linProjMsg = reinterpret_cast<CDOTAUserMsg_DestroyLinearProjectile*>(msg);
		auto handle = linProjMsg->handle();
		linearProjectiles.erase(handle);
		if (TrackedProjectiles.count(handle)) {
			GameSystems::ParticleManager->DestroyParticle(TrackedProjectiles[handle]);
			TrackedProjectiles.erase(handle);
		}
	}
}
