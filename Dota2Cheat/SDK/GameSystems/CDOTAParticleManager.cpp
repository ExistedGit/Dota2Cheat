#include "CDOTAParticleManager.h"

int CDOTAParticleManager::GetParticleCount() {
	return Member<uint32_t>(0x80);
}

CDOTAParticleManager::ParticleContainer** CDOTAParticleManager::GetParticleArray() {
	return Member<ParticleContainer**>(0x88);
}

uint32_t CDOTAParticleManager::GetHandle() {
	return Member<uint32_t>(0xb8);
}

void CDOTAParticleManager::IncHandle() {
	*(uint32_t*)((uintptr_t)this + 0xb8) = GetHandle() + 1;
}

ParticleWrapper CDOTAParticleManager::CreateParticle(const char* name, ParticleAttachment_t attachType, CBaseEntity* ent) {
	static_assert(sizeof(CreateParticleInfo) == 0x40);
	CreateParticleInfo info{};
	info.m_szParticleName = name;
	info.m_particleAttachment = attachType;
	info.m_pTargetEntity = ent;

	auto h = GetHandle();
	IncHandle();
	CallVFunc<VTableIndexes::CDOTAParticleManager::CreateParticle>(h, &info);

	ParticleWrapper result{};
	result.info = info;
	result.particle = GetParticleArray()[GetParticleCount() - 1]->GetParticle();
	result.handle = h;

	particles.push_back(result);

	return result;
}

void CDOTAParticleManager::DestroyParticle(uint32_t handle) {
	DestroyParticleFunc(this, handle, 1);
}

void CDOTAParticleManager::DestroyParticle(ParticleWrapper& particleWrap) {
	if (!HVALID(particleWrap.handle))
		return;
	DestroyParticleFunc(this, particleWrap.handle, 1);
	particleWrap.Invalidate();
}

void CDOTAParticleManager::OnExitMatch() {
	for (auto& pw : particles)
		DestroyParticle(pw);

	particles.clear();
}

