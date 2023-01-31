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

CDOTAParticleManager::Particle* CDOTAParticleManager::CreateParticle(const char* name, ParticleAttachment_t attachType, BaseEntity* ent) {
	CDOTAParticleManager::ParticleInfo info{};
	info.particleName = name;
	info.attachType = attachType;
	info.attachEntity = ent;

	auto h = GetHandle();
	IncHandle();
	CallVFunc<7>(h, &info);

	//ParticleWrapper result{};
	//result.info = info;
	//result.particle = GetParticleArray()[GetParticleCount() - 1]->GetParticle();
	//result.handle = GetHandle();

	return GetParticleArray()[GetParticleCount() - 1]->GetParticle();
}

void CDOTAParticleManager::DestroyParticle(uint32_t handle) {
	Signatures::DestroyParticle(this, handle, 1);
}

void CDOTAParticleManager::DestroyParticle(ParticleWrapper& info) {
	Signatures::DestroyParticle(this, info.handle, 1);
	info.particle = nullptr;
	info.handle = 0XFFFFFFFF;
	info.info = CDOTAParticleManager::ParticleInfo{};
}
