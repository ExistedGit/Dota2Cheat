#include "CDOTAParticleManager.h"

ParticleWrapper CDOTAParticleManager::CreateParticle(const char* name, ParticleAttachment_t attachType, CBaseEntity* ent) {
	static_assert(sizeof(CreateParticleInfo) == 0x40);
	CreateParticleInfo info{};
	info.m_szParticleName = name;
	info.m_particleAttachment = attachType;
	info.m_pTargetEntity = ent;

	CallVFunc<VTableIndexes::CDOTAParticleManager::CreateParticle>(GetHandle(), &info);

	ParticleWrapper result{};
	result.info = info;
	result.particle = GetParticles().last()->GetParticle();
	result.handle = GetHandle()++;

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

