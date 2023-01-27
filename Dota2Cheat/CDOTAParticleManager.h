#pragma once
#include "sdk.h"
#include "Wrappers.h"
class CDOTAParticleManager :public VClass {

public:
	enum class ParticleAttachment_t : int {
		PATTACH_INVALID = -1,
		PATTACH_ABSORIGIN = 0,
		PATTACH_ABSORIGIN_FOLLOW = 1,
		PATTACH_CUSTOMORIGIN = 2,
		PATTACH_CUSTOMORIGIN_FOLLOW = 3,
		PATTACH_POINT = 4,
		PATTACH_POINT_FOLLOW = 5,
		PATTACH_EYES_FOLLOW = 6,
		PATTACH_OVERHEAD_FOLLOW = 7,
		PATTACH_WORLDORIGIN = 8,
		PATTACH_ROOTBONE_FOLLOW = 9,
		PATTACH_RENDERORIGIN_FOLLOW = 10,
		PATTACH_MAIN_VIEW = 11,
		PATTACH_WATERWAKE = 12,
		PATTACH_CENTER_FOLLOW = 13,
		PATTACH_CUSTOM_GAME_STATE_1 = 14,
		PATTACH_HEALTHBAR = 15,
		MAX_PATTACH_TYPES = 16,
	};
	struct ParticleInfo {
		const char* particleName;
		ParticleAttachment_t attachmentType;
		BaseEntity* ent;
	private:
		void* unk0 = nullptr;
		void* unk1 = nullptr;
		void* unk2 = nullptr;
		void* unk3 = nullptr;
		void* unk4 = nullptr;
	};

	class Particle : public VClass {
	public:
		VClass* GetParticleCollection() {
			return Member<VClass*>(0x20);
		}
		Particle* SetControlPoint(int idx, Vector3* pos) {
			auto coll = GetParticleCollection();
			coll->GetVFunc(0x80 / 8)(coll, idx, pos);
			return this;
		}
	};
	struct ParticleContainer {
		Particle* GetParticle() {
			return *(Particle**)((uintptr_t)this + 0x10);
		}
	};
	
	int GetParticleCount() {
		return Member<uint32_t>(0x80);
	}
	ParticleContainer** GetParticleArray() {
		return Member<ParticleContainer**>(0x88);
	}

	uint32_t GetHandle() {
		return Member<uint32_t>(0x98);
	}
	void IncHandle() {
		*(uint32_t*)((uintptr_t)this + 0x98) = GetHandle() + 1;
	}

	Particle* CreateParticle(ParticleInfo info) {
		GetVFunc(7)(this, GetHandle(), info);
		IncHandle();
		return GetParticleArray()[GetParticleCount() - 1]->GetParticle();
	}
};