#pragma once
#include "../Base/Definitions.h"
#include "../Entities/CEntityIdentity.h"
#include <sol/sol.hpp>

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64 // 0x3F
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST * MAX_ENTITY_LISTS // 0x8000

class CEntityIdentities
{
public:
	CEntityIdentity m_pIdentities[MAX_ENTITIES_IN_LIST];
};

class CEntitySystem{
public:
	virtual void n_0() = 0;
	virtual void BuildResourceManifest(void) = 0; // 01
	virtual void n_2() = 0;
	virtual void n_3() = 0;
	virtual void n_4() = 0;
	virtual void n_5() = 0;
	virtual void n_6() = 0;
	virtual void AddRefKeyValues(void const*) = 0; // 7
	virtual void ReleaseKeyValues(void const*) = 0; // 8
	virtual void n_9() = 0;
	virtual void n_10() = 0;
	virtual void ClearEntityDatabase(void); // 11
	virtual CBaseEntity* FindEntityProcedural(const char *...) = 0;
	virtual CBaseEntity* OnEntityParentChanged(CBaseEntity*, CBaseEntity*) = 0; //13
	virtual CBaseEntity* OnAddEntity(CBaseEntity*, ENT_HANDLE) = 0; // 14
	virtual CBaseEntity* OnRemoveEntity(CBaseEntity*, ENT_HANDLE) = 0; // 15
	virtual void n_16() = 0;
	virtual void SortEntities(int, void*, int*, int*) = 0; // 17
	virtual void n_18() = 0;
	virtual void n_19() = 0;
	virtual void n_20() = 0;
	virtual void n_21() = 0;
};

class CGameEntitySystem : public CEntitySystem
{
	void* unk;
	CEntityIdentities* m_pEntityList[MAX_ENTITY_LISTS];
public:
	CEntityIdentity* GetIdentity(int index)
	{
		if (index <= -1 || index >= (MAX_TOTAL_ENTITIES - 1))
			return nullptr;

		CEntityIdentities* chunkToUse = m_pEntityList[(index / MAX_ENTITIES_IN_LIST)]; // equal to ( index >> 9 )

		if (!chunkToUse)
			return nullptr;

		CEntityIdentity* identity = &chunkToUse->m_pIdentities[index % MAX_ENTITIES_IN_LIST]; // equal to ( index & 1FF )

		if (!identity)
			return nullptr;

		// This is an extra check in the official implementation that I am omitting for speed
		//if( (LOWORD( identity->entHandle ) & 0x7FFF) == index ){
		//    return identity->entity;
		//}

		return identity;
	}
	template<typename T = CBaseEntity>
	T* GetEntity(int index)
	{
		auto identity = GetIdentity(index);
		if(identity)
			return (T*)identity->entity;
		return nullptr;
	}
	int GetHighestEntityIndex()
	{		
		// IDA:
		// xref "cl_showents" -> lea rax, [XXXXXXXX] above
		// decompile it, there is a cycle using a variable initialized with the first call(to sub_18XXXXXX)
		// that function will have this function
		return *(int*)((uintptr_t)this + 0x1510);
	}
};

