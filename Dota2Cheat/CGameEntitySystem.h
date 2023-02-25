#pragma once
#include "sdk.h"
#include <functional>

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64 // 0x3F
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST * MAX_ENTITY_LISTS // 0x8000

class BaseEntity;
class BaseNpc;

class CEntityIdentity {

public:
	BaseEntity* entity;
	void* dunno;
	int entHandle; // LOWORD(handle) & 0x7FFF = entID
	int unk2; // always seems to be -1
	const char* internalName; // these two strings are optional!
	const char* entityName; // ex: item_tpscroll
	//void* unkptr2;
private:
	void* unkptr3;
public:
	char flags[4];
private:
	char pad[4];
	void* unkptr4;
	void* unkptr5;
public:
	CEntityIdentity* prevValid;
	CEntityIdentity* nextValid;
private:
	void* unkptr6;
	void* unkptr7;
	void* unkptr8;
public:
	const char* GetName() const {
		return internalName  ? internalName : entityName;
	}
	
	bool IsDormant() const {
		return (flags[0] & 0x80);
	}
	
	uint32_t GetEntIndex() const {
		return H2IDX(entHandle);
	}

	static void BindLua(sol::state& lua) {
		sol::usertype<CEntityIdentity> type = lua.new_usertype<CEntityIdentity>("CEntityIdentity");
		type["entity"] = &CEntityIdentity::entity;
		type["entHandle"] = &CEntityIdentity::entHandle;
		type["internalName"] = &CEntityIdentity::internalName;
		type["entityName"] = &CEntityIdentity::entityName;
		type["flags"] = &CEntityIdentity::flags;
		type["GetName"] = &CEntityIdentity::GetName;
		type["IsDormant"] = &CEntityIdentity::IsDormant;
	}
};

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
	virtual BaseEntity* FindEntityProcedural(const char *...) = 0;
	virtual BaseEntity* OnEntityParentChanged(BaseEntity*, BaseEntity*) = 0; //13
	virtual BaseEntity* OnAddEntity(BaseEntity*, ENT_HANDLE) = 0; // 14
	virtual BaseEntity* OnRemoveEntity(BaseEntity*, ENT_HANDLE) = 0; // 15
	virtual void n_16() = 0;
	virtual void SortEntities(int, void*, int*, int*) = 0; // 17
	virtual void n_18() = 0;
	virtual void n_19() = 0;
	virtual void n_20() = 0;
	virtual void n_21() = 0;
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
	template<typename T = BaseEntity>
	T* GetEntity(int index)
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
		//std::cout << "identity: " << identity << " " << identity->entity << '\n';
		return (T*)identity->entity;
	}
	int GetHighestEntityIndex()
	{
		return *(int*)((uintptr_t)this + 0x1e90);
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CGameEntitySystem>("CGameEntitySystem");
		type["GetIdentity"] = &CGameEntitySystem::GetIdentity;
		type["GetBaseEntity"] = &CGameEntitySystem::GetEntity<BaseEntity>;
		type["GetHighestEntityIndex"] = &CGameEntitySystem::GetHighestEntityIndex;
		type["GetBaseNpc"] = &CGameEntitySystem::GetEntity<BaseNpc>;
	}
	
};

