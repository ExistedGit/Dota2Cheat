#pragma once

#include "CBaseEntity.h"

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64 // 0x3F
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST * MAX_ENTITY_LISTS // 0x8000
#define ClearEntityDatabaseMode_t char
#define CEntityHandle unsigned int
#define EntitySpawnInfo_t char
//#define LOWORD(a) ((uint16_t)(a))
//#define HIWORD(a) ((uint16_t)(((uint32_t)(a) >> 16) & 0xFFFF))
class CBaseEntity;

class CEntityIdentity {

public:
    CBaseEntity *entity;
    void *dunno;
    int entHandle; // LOWORD(handle) & 0x7FFF = entID
    int unk2; // always seems to be -1
    const char* internalName; // these two strings are optional!
    const char* entityName; // ex: item_tpscroll
	void* unkptr2;
	void* unkptr3;
    void* unkptr4;
    void* unkptr5;
    CEntityIdentity *prevValid;
    CEntityIdentity *nextValid;
	void* unkptr6;
	void* unkptr7;
	void* unkptr8;

	const char* GetName() const {
		return internalName != nullptr ? internalName : entityName;
	}
};

class CEntityIdentities
{
public:
    CEntityIdentity m_pIdentities[MAX_ENTITIES_IN_LIST];
};



class CEntitySystem {
public:
	virtual void n_0();
	virtual void BuildResourceManifest(void); // 01
	virtual void n_2();
	virtual void n_3();
	virtual void n_4();
	virtual void n_5();
	virtual void n_6();
	virtual void AddRefKeyValues(void const*); // 7
	virtual void ReleaseKeyValues(void const*); // 8
	virtual void n_9();
	virtual void n_10();
	virtual void ClearEntityDatabase(void); // 11
	virtual CBaseEntity* FindEntityProcedural(const char *...);
	virtual CBaseEntity* OnEntityParentChanged(CBaseEntity*, CBaseEntity*); //13
	virtual CBaseEntity* OnAddEntity(CBaseEntity*, CEntityHandle); // 14
	virtual CBaseEntity* OnRemoveEntity(CBaseEntity*, CEntityHandle); // 15
	virtual void n_16();
	virtual void SortEntities(int, EntitySpawnInfo_t*, int*, int*); // 17
	virtual void n_18();
	virtual void n_19();
	virtual void n_20();
	virtual void n_21();
};

class CGameEntitySystem : public CEntitySystem
{
public:

	void* unk;
	//void* unk2; 
	//void* unk3;, remove because of vtable

	CEntityIdentities* m_pEntityList[MAX_ENTITY_LISTS];
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
	CBaseEntity* GetBaseEntity(int index)
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

		return identity->entity;
	}
	int GetHighestEntityIndex()
	{
		return *(int*)((uintptr_t)this + 0x1e90);
	}
};