#include "CDOTAParticleManager.h"

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

CDOTAParticleManager::ParticleWrapper CDOTAParticleManager::CreateParticle(const char* name, ParticleAttachment_t attachType, BaseEntity* ent) {
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

	return result;
}

void CDOTAParticleManager::DestroyParticle(uint32_t handle) {
	Signatures::DestroyParticle(this, handle, 1);
}

void CDOTAParticleManager::DestroyParticle(ParticleWrapper& particleWrap) {
	Signatures::DestroyParticle(this, particleWrap.handle, 1);
	particleWrap.Invalidate();
}