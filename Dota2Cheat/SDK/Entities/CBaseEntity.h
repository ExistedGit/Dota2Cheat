#pragma once
#include <sol/sol.hpp>

#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/Vector.h"
#include "../Base/Color.h"

#include "../Netvars.h"
#include "../Protobufs/dota_shared_enums.pb.h"

#include "CEntityIdentity.h"

struct CSchemaClassBinding {
	CSchemaClassBinding* parent;
	const char* binaryName; // ex: C_World
	const char* className; // ex: client
	void* classInfoOldSynthesized;
	void* classInfoN;
	void* thisModuleBindingPointer;
	void* pSchemaType;

	static void BindLua(sol::state& lua) {
		sol::usertype<CSchemaClassBinding> type = lua.new_usertype<CSchemaClassBinding>("CSchemaClassBinding");
		type["parent"] = &CSchemaClassBinding::parent;
		type["binaryName"] = &CSchemaClassBinding::binaryName; // ex: C_World
		type["className"] = &CSchemaClassBinding::className; // ex: client
		type["classInfoOldSynthesized"] = &CSchemaClassBinding::classInfoOldSynthesized;
		type["classInfoN"] = &CSchemaClassBinding::classInfoN;
		type["thisModuleBindingPointer"] = &CSchemaClassBinding::thisModuleBindingPointer;
		type["pSchemaType"] = &CSchemaClassBinding::pSchemaType;
	}
};

class CBaseEntity : public VClass {
public:
	CSchemaClassBinding* SchemaBinding() {
		return CallVFunc<0, CSchemaClassBinding*>();
	};
	typedef void(__fastcall* EntityCallback)(void* thisptr);
	static inline EntityCallback OnColorChanged{};

	GETTER(CEntityIdentity*, GetIdentity, Netvars::CEntityInstance::m_pEntity)
	GETTER(int, GetHealth, Netvars::C_BaseEntity::m_iHealth)
	GETTER(int, GetMaxHealth, Netvars::C_BaseEntity::m_iMaxHealth)
	GETTER(DOTA_GC_TEAM, GetTeam, Netvars::C_BaseEntity::m_iTeamNum)
	GETTER(int8_t, GetLifeState, Netvars::C_BaseEntity::m_lifeState)
	GETTER(ENT_HANDLE, GetOwnerEntityHandle, Netvars::C_BaseEntity::m_hOwnerEntity)


	uint32_t GetHandle() {
		return GetIdentity()->entHandle;
	}
	// Returns the index of this entity in the entity system
	uint32_t GetIndex() {
		return H2IDX(GetHandle());
	}

	void SetColor(Color clr)
	{
		uintptr_t clrAddr = (uintptr_t)this + Netvars::C_BaseModelEntity::m_clrRender;
		*(uint8_t*)(clrAddr + 0) = clr.RGBA[0];
		*(uint8_t*)(clrAddr + 1) = clr.RGBA[1];
		*(uint8_t*)(clrAddr + 2) = clr.RGBA[2];
		*(uint8_t*)(clrAddr + 3) = clr.RGBA[3];

		OnColorChanged(this);
	}

	Vector GetPos() {
		// The m_vecAbsOrigin netvar is actually inside a CGameSceneNode
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode) // getting the node
			->Member<Vector>(Netvars::CGameSceneNode::m_vecAbsOrigin); // getting the absOrigin
	}

	// In degrees from 180 to -180(on 0 it looks right)
	float GetRotation() {
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)
			->Member<float>(0xf4);
	}

	// Gets the point in front of the entity at the specified distance
	Vector GetForwardVector(int dist) {
		auto pos = GetPos();
		float rotation = GetRotation() * M_PI_F / 180;

		float sine = sinf(rotation), cosine = cosf(rotation);
		auto forwardVec = Vector(cosine * dist, sine * dist, 0);
		return pos + forwardVec;
	}

	static void BindLua(sol::state& lua) {
		sol::usertype<CBaseEntity> type = lua.new_usertype<CBaseEntity>("CBaseEntity");
		type["GetSchemaBinding"] = &CBaseEntity::SchemaBinding;
		type["GetIdentity"] = &CBaseEntity::GetIdentity;
		type["SetColor"] = &CBaseEntity::SetColor;
		type["GetOwnerEntityHandle"] = &CBaseEntity::GetOwnerEntityHandle;
		type["GetPos"] = &CBaseEntity::GetPos;
		type["GetRotation"] = &CBaseEntity::GetRotation;
		type["GetForwardVector"] = &CBaseEntity::GetForwardVector;
		type["GetMaxHealth"] = &CBaseEntity::GetMaxHealth;
		type["GetHealth"] = &CBaseEntity::GetHealth;
		type["GetTeam"] = &CBaseEntity::GetTeam;
		type["GetLifeState"] = &CBaseEntity::GetLifeState;
	}
};
