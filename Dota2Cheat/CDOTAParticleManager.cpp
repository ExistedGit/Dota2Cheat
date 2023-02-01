#include "CDOTAParticleManager.h"

std::map<TrackedParticles_t, CDOTAParticleManager::ParticleWrapper> CDOTAParticleManager::TrackedParticles{};

int CDOTAParticleManager::GetParticleCount() {
	return Member<uint32_t>(0x80);
}

CDOTAParticleManager::ParticleContainer** CDOTAParticleManager::GetParticleArray() {
	return Member<ParticleContainer**>(0x88);
}

uint32_t CDOTAParticleManager::GetHandle() {
	return Member<uint32_t>(0x98);
}

void CDOTAParticleManager::IncHandle() {
	*(uint32_t*)((uintptr_t)this + 0x98) = GetHandle() + 1;
}

CDOTAParticleManager::ParticleWrapper CDOTAParticleManager::CreateParticle(const char* name, ParticleAttachment_t attachType, BaseEntity* ent, TrackedParticles_t trackType) {
	CDOTAParticleManager::ParticleInfo info{};
	info.particleName = name;
	info.attachType = attachType;
	info.attachEntity = ent;

	auto h = GetHandle();
	IncHandle();
	CallVFunc<7>(h, &info);

	ParticleWrapper result{};
	result.info = info;
	result.particle = GetParticleArray()[GetParticleCount() - 1]->GetParticle();
	result.handle = h;

	if (trackType != TrackedParticles_t::TRACKED_PARTICLE_NOT_TRACKED) {
		auto oldWrap = TrackedParticles[trackType];
		if (oldWrap.particle != nullptr)
			DestroyParticle(oldWrap);
		
		TrackedParticles[trackType] = result;
	}

	return result;
}

void CDOTAParticleManager::DestroyParticle(uint32_t handle) {
	Signatures::DestroyParticle(this, handle, 1);
}

void CDOTAParticleManager::DestroyParticle(ParticleWrapper& particle) {
	Signatures::DestroyParticle(this, particle.handle, 1);
	particle.Invalidate();
}

void CDOTAParticleManager::DestroyTrackedParticle(TrackedParticles_t trackType) {
	if (trackType != TRACKED_PARTICLE_NOT_TRACKED)
		DestroyParticle(TrackedParticles[trackType]);
}
