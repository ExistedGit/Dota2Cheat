#include "CDOTAParticleManager.h"

ParticleWrapper CDOTAParticleManager::CreateParticle(const char* name, ParticleAttachment_t attachType, CBaseEntity* ent) {
	CreateParticleInfo info{};
	info.m_szParticleName = name;
	info.m_particleAttachment = attachType;
	info.m_pTargetEntity = ent;

	CallVFunc<VTableIndexes::CDOTAParticleManager::CreateParticle>(GetHandle(), &info);
	auto container = GetParticles().last();
	ParticleWrapper result{};
	result.info = info;
	result.particle = container->GetParticle();
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

