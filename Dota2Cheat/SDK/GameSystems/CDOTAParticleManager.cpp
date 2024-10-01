#include "CDOTAParticleManager.h"

ParticleWrapper CDOTAParticleManager::CreateParticle(const char* name, ParticleAttachment_t attachType, CBaseEntity* ent) {
	CreateParticleInfo info{};
	info.m_szParticleName = name;
	info.m_particleAttachment = attachType;
	info.m_pTargetEntity = ent;

	uint32_t& handle = Handle();

	GetVFunc(VMI::CDOTAParticleManager::CreateParticle).Call<void, uint32_t>(handle, &info);
	auto container = GetParticles().last();
	ParticleWrapper result{};
	result.info = info;
	result.particle = container->GetParticle();
	result.handle = handle;

	handle += 1;
	
	particles.push_back(result);

	return result;
}

void CDOTAParticleManager::DestroyParticle(uint32_t handle) {
	DestroyParticleFunc(this, handle, false, true);
}

void CDOTAParticleManager::DestroyParticle(ParticleWrapper& particleWrap) {
	if (!HVALID(particleWrap.handle))
		return;
	DestroyParticleFunc(this, particleWrap.handle, false, true);
	particleWrap.Invalidate();
}

void CDOTAParticleManager::OnExitMatch() {
	for (auto& pw : particles)
		DestroyParticle(pw);

	particles.clear();
}

