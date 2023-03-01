#pragma once
#include "Wrappers.h"
#include "Interfaces.h"

#ifdef _DEBUG
inline void LogEntities() {
	for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto* ent = Interfaces::EntitySystem->GetEntity(i);
		if (ent == nullptr)
			continue;
		//std::cout << ent->SchemaBinding() << '\n';
		const char* className = ent->SchemaBinding()->binaryName;
		if (
			className
			//&& strstr(className, "Rune")
			)
			std::cout << className << ' ' << i
			//<< " // " << ent->GetPos2D().x << ' ' << ent->GetPos2D().y
			<< " -> " << ent << '\n';
	}
}
inline void LogModifiers(BaseNpc* npc) {
	std::cout << "modifiers:\n";
	for (const auto& modifier : npc->GetModifierManager()->GetModifierList())
		std::cout << "\t" << modifier->GetName() << ' ' << modifier << '\n';
}
inline void LogInvAndAbilities(BaseNpc* npc = nullptr) {
	if (npc == nullptr)
		npc = ctx.assignedHero;

	std::cout << std::dec;
	std::cout << "abilities: " << '\n';
	for (const auto& ability : npc->GetAbilities()) {
		if (ability.name) {

			std::cout << '\t' << ability.name << " " << H2IDX(ability.handle)
				//<< " CD: " << ability.GetAs<BaseAbility>()->GetCooldown() 
				//<< ' ' << std::dec << ability.GetAs<BaseAbility>()->GetEffectiveCastRange()
				<< ' ' << ability.GetEntity();

			std::cout << '\n';

		}
	}
	std::cout << "inventory: " << '\n';
	for (const auto& item : npc->GetItems()) {
		if (item.name)
			std::cout << '\t' << item.name << " " << H2IDX(item.handle)
			<< ' ' << item.GetEntity()
			<< '\n';
	}
}

namespace Test {
	inline CDOTAParticleManager::ParticleWrapper particleWrap{};
	inline std::unique_ptr<VMT> ParticleManagerVMT;
	inline std::map<VClass*, std::unique_ptr<VMT>> partMap{};

	void hkSetControlPoint(VClass* thisptr, int idx, Vector3* pos) {
		partMap[thisptr]->GetOriginalMethod<decltype(&hkSetControlPoint)>(16)(thisptr, idx, pos);
		//std::cout << "Set CP #" << idx << ": " << *pos << '\n';
	}
	void hkCreateParticle(CDOTAParticleManager* thisptr, uint32_t handle, CDOTAParticleManager::ParticleInfo* info) {
		ParticleManagerVMT->GetOriginalMethod<decltype(&hkCreateParticle)>(7)(thisptr, handle, info);
		if (!strcmp(info->particleName, "particles/units/heroes/hero_primal_beast/primal_beast_onslaught_range_finder.vpcf")) {
			auto newParticleCollection = Globals::ParticleManager->GetParticleArray()[Globals::ParticleManager->GetParticleCount() - 1]->GetParticle()->GetParticleCollection();
			//std::cout << "Created particle: " << info->particleName << '\n';
			partMap[newParticleCollection] = std::unique_ptr<VMT>(new VMT(newParticleCollection));
			partMap[newParticleCollection]->HookVM(hkSetControlPoint, 16);
			partMap[newParticleCollection]->ApplyVMT();
		}
	}
	void HookParticles() {
		ParticleManagerVMT = std::unique_ptr<VMT>(new VMT(Globals::ParticleManager));
		ParticleManagerVMT->HookVM(hkCreateParticle, 7);
		ParticleManagerVMT->ApplyVMT();
	}
}
#endif // _DEBUG
