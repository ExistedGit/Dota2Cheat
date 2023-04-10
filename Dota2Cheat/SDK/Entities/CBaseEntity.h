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

	static void BindLua(sol::state& lua);
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
		auto id = GetIdentity();
		if (!IsValidReadPtr(id))
			return INVALID_HANDLE;
		return id->entHandle;
	}
	// Returns the index of this entity in the entity system
	uint32_t GetIndex();

	void SetColor(Color clr);

	Vector GetPos();

	// In degrees from 180 to -180(on 0 it looks right)
	float GetRotation();

	// Gets the point in front of the entity at the specified distance
	Vector GetForwardVector(float dist);

	static void BindLua(sol::state& lua);
};
