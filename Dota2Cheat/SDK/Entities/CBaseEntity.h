#pragma once
#include "../Protobufs/dota_shared_enums.pb.h"


#include "../Base/VClass.h"
#include "../Base/NormalClass.h"
#include "../Base/Definitions.h"
#include "../Base/Vector.h"
#include "../Base/Color.h"

#include "../Netvars.h"

#include "CEntityIdentity.h"

class CDOTAPlayerController;

struct CSchemaClassBinding {
	const char
		* binaryName, // ex: C_DOTA_Unit_Hero_Nevermore
		* serverName; // ex: CDOTA_Unit_Hero_Nevermore

	void* parent;
	const char* fullName; // ex: client.dll!C_DOTA_Unit_Hero_Nevermore
	void* idk;

	int listIndex;
	PAD(4 + 8 * 2);
};

class CBaseEntity : public VClass {
public:
	struct CModelState : public NormalClass {
		FIELD(uint64_t, MeshGroupMask, Netvars::CModelState::m_MeshGroupMask);
		GETTER(const char*, GetModelName, Netvars::CModelState::m_ModelName);
		auto GetModel() {
			return *Member<NormalClass**>(Netvars::CModelState::m_hModel);
		};
	};
	struct CSkeletonInstance : public VClass {
		// reversed from xref: "CBaseModelEntity::SetBodygroup(%d,%d) failed: CBaseModelEntity has no model!\n"
		// last two subs are get and set
		GETTER(CBaseEntity*, GetOwner, Netvars::CGameSceneNode::m_pOwner);
		IGETTER(CModelState, GetModelState, Netvars::CSkeletonInstance::m_modelState)
	};

	CSchemaClassBinding* SchemaBinding() {
		return CallVFunc<0x118 / 8, CSchemaClassBinding*>();
	};

	inline static void(__fastcall* OnColorChanged)(void*) = {};

	GETTER(CEntityIdentity*, GetIdentity, Netvars::CEntityInstance::m_pEntity);
	GETTER(int, GetHealth, Netvars::C_BaseEntity::m_iHealth);
	GETTER(int, GetMaxHealth, Netvars::C_BaseEntity::m_iMaxHealth);
	GETTER(DOTA_GC_TEAM, GetTeam, Netvars::C_BaseEntity::m_iTeamNum);
	GETTER(int8_t, GetLifeState, Netvars::C_BaseEntity::m_lifeState);
	GETTER(CHandle<CDOTAPlayerController>, GetOwnerEntityHandle, Netvars::C_BaseEntity::m_hOwnerEntity);
	GETTER(CSkeletonInstance*, GetGameSceneNode, Netvars::C_BaseEntity::m_pGameSceneNode);

	const char* GetModelName() {
		// og's explanation:
		// CModelState has 3 CStrongHandle pointers at 0xA0 and below
		// These strong handles have a model pointer and its name
		return GetGameSceneNode()->GetModelState()->GetModelName();
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
	uint32_t GetIndex() {
		return H2IDX(GetHandle());
	}

	void SetColor(Color clr)
	{
		Field<Color>(Netvars::C_BaseModelEntity::m_clrRender) = clr;
		OnColorChanged(this);
	}

	float& ModelScale() {
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)
			->Field<float>(Netvars::CGameSceneNode::m_flScale);
	}

	Vector GetPos() {
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)
			->Member<Vector>(Netvars::CGameSceneNode::m_vecAbsOrigin);
	}

	// In degrees from 180 to -180(on 0 it looks right)
	float GetRotation() {
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)
			->Member<Vector>(Netvars::CGameSceneNode::m_angRotation).y;
	}

	// Gets the point in front of the entity at the specified distance
	Vector GetForwardVector(float dist) {
		auto pos = GetPos();
		float rotation = GetRotation() * M_PI / 180;

		float sine = sinf(rotation), cosine = cosf(rotation);
		auto forwardVec = Vector(cosine * dist, sine * dist, 0);
		return pos + forwardVec;
	}
};
