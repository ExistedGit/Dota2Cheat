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
	info.attachType = attachType;
	info.attachEntity = ent;
	info.particleName = name;
	info.unk1 = ent;

	IncHandle();
	CallVFunc<7>(GetHandle(), &info, Interfaces::Entity->m_pEntityList);

	ParticleWrapper result{};
	result.info = info;
	result.particle = GetParticleArray()[GetParticleCount() - 1]->GetParticle();
	result.handle = GetHandle();

	return result;
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
