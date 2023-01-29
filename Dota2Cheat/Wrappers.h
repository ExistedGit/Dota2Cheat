#pragma once
#include <cstdint>
#include "Schema.h"
#include "Signatures.h"
#include "CUtlVector.h"
#include "SDK/color.h"

struct SchemaClassBinding {
	SchemaClassBinding* parent;
	const char* binaryName; // ex: C_World
	//const char* projectName; // ex: libclient.so
	const char* className; // ex: client
	void* classInfoOldSynthesized;
	void* classInfoN;
	void* thisModuleBindingPointer;
	void* pSchemaType;
};

class CUnitInventory {
public:
	// Returns an array of 19 entity handles representing slots, if the slot is empty, the handle is invalid(0XFFFFFFFF)
	// Valid handles of items are ordered by slots, i. e. moving an item to backpack will change its index inside this array
	ENT_HANDLE* GetItems() {
		static int offset = Schema::Netvars["C_DOTA_UnitInventory"]["m_hItems"];
		if (!offset)
			return nullptr;
		return reinterpret_cast<ENT_HANDLE*>((uintptr_t)this + offset);
	}
};


class BaseEntity : public VClass {
public:
	inline CSchemaClassBinding* SchemaBinding() {
		return (CSchemaClassBinding*)GetVFunc(0)();
	};

	inline CEntityIdentity* GetIdentity() {
		return Member<CEntityIdentity*>(0x10);
	}

	inline void SetColor(Color clr)
	{
		static int offset = Schema::Netvars["C_BaseModelEntity"]["m_clrRender"];
		if (!offset)
			return;
		uintptr_t clrAddr = (uintptr_t)this + offset;
		*(BYTE*)(clrAddr + 0) = static_cast<BYTE>(clr.RGBA[0]);
		*(BYTE*)(clrAddr + 1) = static_cast<BYTE>(clr.RGBA[1]);
		*(BYTE*)(clrAddr + 2) = static_cast<BYTE>(clr.RGBA[2]);
		*(BYTE*)(clrAddr + 3) = static_cast<BYTE>(clr.RGBA[3]);

		Signatures::OnColorChanged(this);
	}
	inline ENT_HANDLE GetOwnerEntityHandle() {
		return Member<ENT_HANDLE>(Schema::Netvars["C_BaseEntity"]["m_hOwnerEntity"]);
	}

	inline Vector3 GetPos() {
		u64 offset = Schema::Netvars["C_BaseEntity"]["m_pGameSceneNode"];
		if (!offset)
			return Vector3::Zero;
		// The m_vecAbsOrigin netvar is actually inside a CGameSceneNode
		return Member<VClass*>(offset) // getting the node
			 ->Member<Vector3>(0x10); // getting the absOrigin
	}
	 
	inline Vector2 GetPos2D() {
		auto vec = GetPos();
		return Vector2(vec.x, vec.y);
	}
	inline int GetMaxHealth() {
		return Member<int>(Schema::Netvars["C_BaseEntity"]["m_iMaxHealth"]);
	}
	inline int GetHealth() {
		return Member<int>(Schema::Netvars["C_BaseEntity"]["m_iHealth"]);
	}
	inline DOTATeam_t GetTeam() {
		return Member<DOTATeam_t>(Schema::Netvars["C_BaseEntity"]["m_iTeamNum"]);
	}
};
class BaseNpc : public BaseEntity {
public:
	struct ItemOrAbility {
		const char* name;
		ENT_HANDLE handle;
		ItemOrAbility(const char* name, ENT_HANDLE handle) :name(name), handle(handle) {

		}
		BaseEntity* GetEntity() const {
			return Interfaces::Entity->GetEntity(ENTID_FROM_HANDLE(handle));
		}
		template<typename T>
		T* GetAs() const {
			return reinterpret_cast<T*>(Interfaces::Entity->GetEntity(ENTID_FROM_HANDLE(handle)));
		}
	};
	// Wrapper function combining the following conditions: 
	// Is not dormant
	// Is alive
	// Is not waiting to spawn
	inline bool IsTargetable() {
		return !GetIdentity()->IsDormant() && GetLifeState() == 0 && !IsWaitingToSpawn();
	}
	inline bool IsWaitingToSpawn() {
		return Member<bool>(Schema::Netvars["C_DOTA_BaseNPC"]["m_bIsWaitingToSpawn"]);
	}
	inline bool IsAncient() {
		return Member<bool>(Schema::Netvars["C_DOTA_BaseNPC"]["m_bIsAncient"]);
	}
	inline float GetSSC() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flStartSequenceCycle"]);
	}
	inline ENT_HANDLE GoalEntity() {
		return Member<ENT_HANDLE>(Schema::Netvars["C_DOTA_BaseNPC"]["m_hGoalEntity"]);
	}
	inline const char* GetUnitName() {
		//return *(const char**)((uintptr_t)this + Schema::Netvars["C_DOTA_BaseNPC"]["m_iszUnitName"]);
		//std::cout << std::hex << Schema::Netvars["C_DOTA_BaseNPC"]["m_iszUnitName"] << '\n';
		//std::cout << "this: " << this << '\n';
		return Member<const char*>(Schema::Netvars["C_DOTA_BaseNPC"]["m_iszUnitName"]);
	}
	inline bool Hero_IsIllusion() {
		return Member<ENT_HANDLE>(Schema::Netvars["C_DOTA_BaseNPC_Hero"]["m_hReplicatingOtherHeroModel"]) != 0xFFFFFFFF;
	}
	inline std::vector<ItemOrAbility> GetAbilities() {
		static int offset = Schema::Netvars["C_DOTA_BaseNPC"]["m_hAbilities"];
		if (!offset)
			return {};

		std::vector<ItemOrAbility> result{};
		ENT_HANDLE* hAbilities = (ENT_HANDLE*)((uintptr_t)this + offset);
		for (int j = 0; j < 35; j++) {
			ENT_HANDLE handle = hAbilities[j];
			if (ENT_HANDLE_VALID(handle)) {
				auto* identity = Interfaces::Entity->GetIdentity(ENTID_FROM_HANDLE(handle));
				result.push_back(ItemOrAbility((identity->entityName != nullptr ? identity->entityName : identity->internalName), identity->entHandle));
			}
		}
		return result;
	}

	inline ItemOrAbility FindItemBySubstring(const char* str) {
		if (str == nullptr)
			return ItemOrAbility{ nullptr, 0xFFFFFFFF };
		for (const auto& item : GetItems())
			if (item.name != nullptr &&
				strstr(item.name, str))
				return item;

		return ItemOrAbility{ nullptr, 0xFFFFFFFF };
	}

	inline std::vector<ItemOrAbility> GetItems() {
		static int offset = Schema::Netvars["C_DOTA_BaseNPC"]["m_Inventory"];
		if (!offset)
			return {};

		std::vector<ItemOrAbility> result{};
		CUnitInventory* inv = (CUnitInventory*)((uintptr_t)this + offset);
		if (inv) {
			ENT_HANDLE* itemsHandle = inv->GetItems();
			for (int i = 0; i < 19; i++) {
				if (ENT_HANDLE_VALID(itemsHandle[i])) {
					auto* identity = Interfaces::Entity->GetIdentity(ENTID_FROM_HANDLE(itemsHandle[i]));
					result.push_back(ItemOrAbility((identity->entityName != nullptr ? identity->entityName : identity->internalName), identity->entHandle));
				}
			}
		}
		return result;
	}

	inline float GetMana() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flMana"]);
	}
	inline float GetMaxMana() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flMaxMana"]);
	}
	inline INT8 GetLifeState() {
		return Member<INT8>(Schema::Netvars["C_BaseEntity"]["m_lifeState"]);
	}
};
class BaseAbility :public BaseEntity {
public:
	float GetCooldown() {
		return Member<float>(0x5a8);
	}
	int GetCharges() {
		return Member<int>(0x610);
	}
	int GetManaCost() {
		return Member<int>(0x5b0);
	}
};

class ItemRune : public BaseEntity {
public:
	DotaRunes GetRuneType() {
		return Member<DotaRunes>(0x990);
	}
};

class DotaPlayer :public BaseEntity {
public:
	inline uint32_t GetAssignedHeroHandle() {
		return Member< uint32_t>(Schema::Netvars["C_DOTAPlayerController"]["m_hAssignedHero"]);
	}
	inline CUtlVector<uint32_t> GetSelectedUnits() {
		return Member<CUtlVector<uint32_t>>(Schema::Netvars["C_DOTAPlayerController"]["m_nSelectedUnits"]);
	}
	inline BaseNpc* GetAssignedHero() {
		return Interfaces::Entity->GetEntity<BaseNpc>(ENTID_FROM_HANDLE(GetAssignedHeroHandle()));
	}
	inline uint64_t GetSteamID() {
		return Member<uint64_t>(0x6b8);
	}
	inline void CastNoTarget(ENT_HANDLE handle, BaseEntity* issuer = nullptr) {
		if (issuer == nullptr)
			issuer = GetAssignedHero();
		PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_NO_TARGET,
			0,
			&Vector3::Zero,
			ENTID_FROM_HANDLE(handle),
			PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			issuer,
			false,
			true
		);
	}
	inline void BuyItem(int itemId) {
		PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_PURCHASE_ITEM,
			1,
			&Vector3::Zero,
			itemId,
			PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			GetAssignedHero(),
			false,
			true
		);
	}
	inline void OrderMoveTo(Vector3* pos, bool directMovement = false, BaseEntity* issuer = nullptr) {
		if (issuer == nullptr)
			issuer = GetAssignedHero();

		PrepareOrder(directMovement ? DotaUnitOrder_t::DOTA_UNIT_ORDER_MOVE_TO_DIRECTION : DotaUnitOrder_t::DOTA_UNIT_ORDER_MOVE_TO_POSITION,
			0,
			pos,
			0,
			PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			issuer,
			false,
			true
		);
	}
	inline void PrepareOrder(DotaUnitOrder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue = false, bool showEffects = true) {
		//if (issuer == nullptr)
		//	issuer = GetAssignedHero();

		if (Signatures::PrepareUnitOrders == nullptr)
			return;

		Signatures::PrepareUnitOrders(this, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
	}
};


extern DotaPlayer* localPlayer;
extern BaseNpc* assignedHero;
extern std::vector<DotaPlayer*> players;