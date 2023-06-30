#pragma once
#include "../Protobufs/dota_shared_enums.pb.h"
#include <sol/sol.hpp>

#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/Vector.h"
#include "../Base/Color.h"

#include "../Netvars.h"

#include "CEntityIdentity.h"

struct CSchemaClassBinding {
	CSchemaClassBinding* parent;
	const char* binaryName; // ex: C_World
	const char* className; // ex: client
	void* classInfoOldSynthesized;
	void* classInfoN;
	void* thisModuleBindingPointer;
	void* pSchemaType;

	static void BindLua(sol::state& lua);
};

class CBaseEntity : public VClass {
public:
	struct CModelState : public NormalClass {
		GETTER(uint64_t, GetMeshGroupMask, Netvars::CModelState::m_MeshGroupMask)
	};
	struct CSkeletonInstance : public VClass {
		//reversed from xref: "CBaseModelEntity::SetBodygroup(%d,%d) failed: CBaseModelEntity has no model!\n"
		// last two subs are get and set
		// GETTER(uint64_t, GetMeshGroupMask, 0x2e0);

		IGETTER(CModelState, GetModelState, Netvars::CSkeletonInstance::m_modelState)
	};

	CSchemaClassBinding* SchemaBinding() {
		return CallVFunc<0, CSchemaClassBinding*>();
	};
	typedef void(__fastcall* EntityCallback)(void* thisptr);
	static inline EntityCallback OnColorChanged{};

	GETTER(CEntityIdentity*, GetIdentity, Netvars::CEntityInstance::m_pEntity);
	GETTER(int, GetHealth, Netvars::C_BaseEntity::m_iHealth);
	GETTER(int, GetMaxHealth, Netvars::C_BaseEntity::m_iMaxHealth);
	GETTER(DOTA_GC_TEAM, GetTeam, Netvars::C_BaseEntity::m_iTeamNum);
	GETTER(int8_t, GetLifeState, Netvars::C_BaseEntity::m_lifeState);
	GETTER(ENT_HANDLE, GetOwnerEntityHandle, Netvars::C_BaseEntity::m_hOwnerEntity);
	GETTER(CSkeletonInstance*, GetGameSceneNode, Netvars::C_BaseEntity::m_pGameSceneNode);

	const char* GetModelName() {
		// og's explanation:
		// CSkeletonInstance has 3 CStrongHandle pointers at 0x200 and below
		// These strong handles have a model pointer and its name
		return *GetGameSceneNode()->Member<NormalClass*>(0x200)->Member<const char**>(8);
	}

	bool IsSameTeam(CBaseEntity* other) {
		return GetTeam() == other->GetTeam();
	}
	uint32_t GetHandle() {
		auto id = GetIdentity();
		if (!IsValidReadPtr(id))
			return INVALID_HANDLE;
		return id->entHandle;
	}
	// Returns the index of this entity in the entity system
	uint32_t GetIndex();

	void SetColor(Color clr);

	float& ModelScale() {
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)->Field<float>(Netvars::CGameSceneNode::m_flScale);
	}

	Vector GetPos();

	// In degrees from 180 to -180(on 0 it looks right)
	float GetRotation();

	// Gets the point in front of the entity at the specified distance
	Vector GetForwardVector(float dist);

	static void BindLua(sol::state& lua);
};
