#include "LinearProjectileWarner.h"


// Draws a dashed red line from begin to end
// Returns the wrapper for the created particle

ParticleWrapper Modules::M_LinearProjectileWarner::DrawTrajectory(const Vector& begin, const Vector& end) {
	auto pw = CParticleMgr::Get()->CreateParticle(
		"particles/ui_mouseactions/range_finder_tower_line.vpcf",
		PATTACH_WORLDORIGIN,
		nullptr
	);
	pw.particle
		->SetControlPoint(2, begin)
		->SetControlPoint(6, { 1,0,0 })
		->SetControlPoint(7, end);
	return pw;
}

void Modules::M_LinearProjectileWarner::OnModifierAdded(CDOTAModifier* modifier) {
	if (!Config::ShowLinearProjTrajectory)
		return;

	auto owner = modifier->GetOwner();
	if (!AbilityTrajectories.count(modifier->GetName()) ||
		EntityTrajectories.count(modifier))
		return;

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

void Modules::M_LinearProjectileWarner::OnModifierRemoved(CDOTAModifier* modifier) {
	if (!EntityTrajectories.count(modifier))
		return;

	CParticleMgr::Get()->DestroyParticle(EntityTrajectories[modifier].particleWrap);
	EntityTrajectories.erase(modifier);
}

void Modules::M_LinearProjectileWarner::OnFrame() {
	for (auto& [modifier, info] : EntityTrajectories) {
		auto owner = modifier->GetOwner();
		auto forwardVec = modifier->GetOwner()->GetForwardVector(info.offset);
		info.particleWrap.particle->SetControlPoint(7, forwardVec);
	}
}

void Modules::M_LinearProjectileWarner::OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID == DOTA_UM_CreateLinearProjectile) {
		if (!Config::ShowLinearProjTrajectory)
			return;

		auto linProjMsg = reinterpret_cast<CDOTAUserMsg_CreateLinearProjectile*>(msg);

		auto newProj = LinearProjectile{
			.source = CEntSys::Get()->GetEntity(linProjMsg->entindex()),
			.handle = linProjMsg->handle(),
			.distance = linProjMsg->distance(),
			.velocity = linProjMsg->velocity(),
			.acceleration = linProjMsg->acceleration(),
			.origin = linProjMsg->origin()
		};


		if (!newProj.source || newProj.source->IsSameTeam(ctx.localHero))
			return;

		auto ratio = newProj.distance / newProj.velocity.Length();
		auto endPoint = newProj.origin;
		endPoint.x += (newProj.velocity * ratio).x;
		endPoint.y += (newProj.velocity * ratio).y;
		TrackedProjectiles[newProj.handle] = DrawTrajectory(newProj.origin, endPoint);

	}
	else if (msgHandle->messageID == DOTA_UM_DestroyLinearProjectile) {
		auto linProjMsg = reinterpret_cast<CDOTAUserMsg_DestroyLinearProjectile*>(msg);
		auto handle = linProjMsg->handle();

		if (TrackedProjectiles.count(handle)) {
			CParticleMgr::Get()->DestroyParticle(TrackedProjectiles[handle]);
			TrackedProjectiles.erase(handle);
		}
	}
}
