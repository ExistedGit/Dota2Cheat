#pragma once
#include "../Base/VClass.h"
#include "../Base/NormalClass.h"
#include "../Base/Definitions.h"
#include "../Base/CUtlVector.h"
#include "../Interfaces/Network/CNetworkMessages.h"
#include "sol/sol.hpp"
#include "../Base/Vector.h"
#include "../VTableIndexes.h"

class CBaseEntity;

// Enum from animationsystem.dll, dumped by Liberalist
enum ParticleAttachment_t : int32_t {
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

// Struct used when creating a particle
class CRecipientFilter
{
public:
	char                pad_0x0000[0x8];   // 0x0000
	NetChannelBufType_t m_nBufType;        // 0x0008
	unsigned char       m_bInitMessage;    // 0x000C
	char                pad_0x000D[0x3];   // 0x000D
	CUtlVector<int>     m_Recipients;      // 0x0010
	char _pad0[0x108];                     // 0x0028
	unsigned char m_bUsingPredictionRules; // 0x0130
	unsigned char m_bIgnorePredictionCull; // 0x0131
	char _pad1[0x6];                       // 0x0132
};                                         // Size=0x0138

struct CreateParticleInfo
{
	const char* m_szParticleName;
	ParticleAttachment_t m_particleAttachment;
	CBaseEntity* m_pTargetEntity;
private:
	void* unk[5] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	};
};

struct CNewParticleEffect : public VClass {

	VClass* GetParticleCollection() {
		return Member<VClass*>(0x20);
	}
	CNewParticleEffect* SetControlPoint(int idx, Vector* pos) {
		auto coll = GetParticleCollection();
		coll->CallVFunc<VTableIndexes::CParticleCollection::SetControlPoint>(idx, pos);
		return this;
	}
	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CNewParticleEffect>("Particle");
		type["SetControlPoint"] = &CNewParticleEffect::SetControlPoint;
	}
};

struct ParticleWrapper {
	CreateParticleInfo info{};
	CNewParticleEffect* particle{};
	ENT_HANDLE handle = INVALID_HANDLE;

	void Invalidate() {
		particle = nullptr;
		handle = INVALID_HANDLE;
		info = CreateParticleInfo{};
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<ParticleWrapper>("ParticleWrapper");
		type["info"] = &ParticleWrapper::info;
		type["particle"] = &ParticleWrapper::particle;
		type["handle"] = &ParticleWrapper::handle;
	}
};

// Found via x64dbg
// Xref "CreateParticle" to a lea rax instruction
// You must see "pParticleName" below it
// Right above "pParticleName" is lea rcx, [XXXXXXXXX], Right click -> Follow in Disassembler -> Constant
// The second call instruction is GetParticleManager, has only mov rax, [XXXXXXXX] then ret
// Now the only remaining step is to get the absolute address of the call, then of the CDOTAParticleManager**(yes, it's a pointer to a pointer, must be dereferenced as seen in Globals.h)

// In the next, third call, we see:
// mov rax, [rcx]       <- Puts the Particle vtable pointer into rax
// call [rax + 38h]     <- Calls the vfunc on index 0x38 / 8 = 16
class CDOTAParticleManager : public VClass {
	static inline std::vector<ParticleWrapper> particles{};
public:
	struct ParticleContainer : NormalClass {
		inline CNewParticleEffect* GetParticle() {
			return Member<CNewParticleEffect*>(0x10);
		}
	};
	typedef void(__fastcall* DestroyParticleFn)(void* thisptr, ENT_HANDLE handle, bool unk);
	static inline DestroyParticleFn DestroyParticleFunc{};


	int GetParticleCount();
	ParticleContainer** GetParticleArray();

	uint32_t GetHandle();
	void IncHandle();

	ParticleWrapper CreateParticle(const char* name, ParticleAttachment_t attachType, CBaseEntity* ent);
	void DestroyParticle(uint32_t handle);
	void DestroyParticle(ParticleWrapper& info);

	void OnExitMatch();

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CDOTAParticleManager>(
			"CDOTAParticleManager"
			);
		type["GetParticleCount"] = &CDOTAParticleManager::GetParticleCount;
		type["CreateParticle"] = &CDOTAParticleManager::CreateParticle;

		type["DestroyParticle"] = sol::overload(
			sol::resolve<void(uint32_t)>(&CDOTAParticleManager::DestroyParticle),
		sol::resolve<void(ParticleWrapper&)>(&CDOTAParticleManager::DestroyParticle)
		);
	}
};

