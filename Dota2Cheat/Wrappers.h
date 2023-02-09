#pragma once
#include <cstdint>
#include "Schema.h"
#include "Signatures.h"
#include "CUtlVector.h"
#include <set>
//#include <span>
#include "SDK/color.h"

#define u64 unsigned long long

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
	bool IsItemInSlot(ENT_HANDLE item, uint32_t slot) {
		if (slot > 18)
			return false;

		return GetItems()[slot] == item;
	}
	int GetItemSlot(ENT_HANDLE item) {
		for (int i = 0; i < 19; i++) {
			if (GetItems()[i] == item)
				return i;
		}
		return -1;
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

	// In degrees from 180 to -180(on 0 it looks right)
	inline float GetRotation() {
		u64 offset = Schema::Netvars["C_BaseEntity"]["m_pGameSceneNode"];
		if (!offset)
			return 0;
		return Member<VClass*>(offset)
			->Member<float>(0x28) * 180;
	}

	// Gets the point in front of the entity at the specified distance
	inline Vector3 GetForwardVector(int dist) {
		auto pos = GetPos();
		float rotation = GetRotation() * PI / 180;

		float sine = sinf(rotation), cosine = cosf(rotation);
		auto forwardVec = Vector3(dist, 0, 0);
		forwardVec.x = cosine * forwardVec.x - sine * forwardVec.y;
		forwardVec.y = sine * forwardVec.x + cosine * forwardVec.y;

		return pos + forwardVec;
	}

	inline Vector2 GetPos2D() {
		auto vec = GetPos();
		return *(Vector2*)&vec;
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
	inline INT8 GetLifeState() {
		return Member<INT8>(Schema::Netvars["C_BaseEntity"]["m_lifeState"]);
	}
};


class ItemRune : public BaseEntity {
public:
	DotaRunes GetRuneType() {
		return Member<DotaRunes>(0x990);
	}
};

class DotaModifier : public VClass {
public:
	inline const char* GetName() {
		return Member<const char*>(0x28);
	}
};

class DotaModifierManager : public VClass {
public:
	// Returns the original CUtlVector that stores the list
	inline CUtlVector<DotaModifier*>* GetModifierListRaw() {
		return (CUtlVector<DotaModifier*>*)((uintptr_t)this + 0x10);
	}
	inline std::vector<DotaModifier*> GetModifierList() {
		auto result = std::vector<DotaModifier*>{};

		auto vecModifiers = (CUtlVector<DotaModifier*>*)((uintptr_t)this + 0x10);
		for (int i = 0; i < vecModifiers->m_Size; i++)
			result.push_back(vecModifiers->at(i));

		return result;
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
			return Interfaces::EntitySystem->GetEntity(H2IDX(handle));
		}
		template<typename T>
		T* GetAs() const {
			return reinterpret_cast<T*>(Interfaces::EntitySystem->GetEntity(H2IDX(handle)));
		}
	};

	inline DotaModifierManager* GetModifierManager() {
		// Inlined into the object instead of a pointer
		return (DotaModifierManager*)((uintptr_t)this + Schema::Netvars["C_DOTA_BaseNPC"]["m_ModifierManager"]);
	}

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

	//Implemented as a method returning a bool rather than a field
	inline bool IsRoshan() {
		return CallVFunc<57, bool>();
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
	inline std::vector<ItemOrAbility> GetAbilities() {
		static int offset = Schema::Netvars["C_DOTA_BaseNPC"]["m_hAbilities"];
		if (!offset)
			return {};

		std::vector<ItemOrAbility> result{};
		ENT_HANDLE* hAbilities = (ENT_HANDLE*)((uintptr_t)this + offset);
		for (int j = 0; j < 35; j++) {
			ENT_HANDLE handle = hAbilities[j];
			if (HVALID(handle)) {
				auto* identity = Interfaces::EntitySystem->GetIdentity(H2IDX(handle));
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

	inline CUnitInventory* GetInventory() {
		static int offset = Schema::Netvars["C_DOTA_BaseNPC"]["m_Inventory"];
		if (!offset)
			return nullptr;
		return (CUnitInventory*)((uintptr_t)this + offset);
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
				if (HVALID(itemsHandle[i])) {
					auto* identity = Interfaces::EntitySystem->GetIdentity(H2IDX(itemsHandle[i]));
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

	inline float GetBaseAttackTime() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flBaseAttackTime"]);
	}
	inline float GetAttackSpeed() {
		return 1 + CallVFunc<295, float>();
	}
};

class BaseNpcHero : public BaseNpc {
public:
	struct HeroAttributes {
		float strength, agility, intellect;
	};

	inline HeroAttributes GetAttributes() {
		return Member<HeroAttributes>(Schema::Netvars["C_DOTA_BaseNPC_Hero"]["m_flStrengthTotal"]);
	}

	inline bool IsIllusion() {
		return Member<ENT_HANDLE>(Schema::Netvars["C_DOTA_BaseNPC_Hero"]["m_hReplicatingOtherHeroModel"]) != 0xFFFFFFFF;
	}
};

// Current stat of Power Treads/Vambrace
enum class ItemStat_t {
	STRENGTH = 0,
	INTELLIGENCE,
	AGILITY
};

class BaseAbility : public BaseEntity {
public:
	inline float GetCooldown() {
		return Member<float>(0x5a8);
	}
	inline int GetCharges() {
		return Member<int>(0x610);
	}
	inline int GetManaCost() {
		return Member<int>(0x5b0);
	}

	inline ItemStat_t GetItemStat() {
		return Member<ItemStat_t>(0x670);
	}

	//inline int GetCastRange() {
	//	auto pos = GetPos();
	//	return CallVFunc<0x798/8, int>(&pos, nullptr, *(uintptr_t*)this);
	//}

	// Xref "Script_GetCastRangeBonus" in x64dbg to a lea rax
	// Below there will be "hTarget" or something, above that will be a lea rcx, [XXX] with a function
	// At the end there are two calls to [rdi + 0x798] and [rdi + 0x7a0], first it gets the range, then the bonus
	// I currently could not get GetCastRange to work as a standalone vfunc
	// found via dynamical analysis
	inline int GetCastRangeBonus() {
		return CallVFunc<0x7A0 / 8, int>(nullptr, nullptr, nullptr);
	}
	inline int GetEffectiveCastRange() {
		return GetCastRange() + GetCastRangeBonus();
	}

	// A bit tricky to reverse, done via x64dbg
	// Xref GetCastRange to a lea, rcx instruction, before it is a lea r9, [XXX] <- Follow in Disassembler > Constant: XXX
	// The return register for x64 fastcall is RAX, so breakpoint it and call with nullptr and entity index of your item
	// Step over until you see RAX change to the hex representation of the item's range(in my case Hand of Midas has 600 dec 0x258 hex)
	// There will be a call instruction that will change RAX to the radius, double-click it
	// At the end of the call are:
	// mov rbx, [rcx + 0x568]     <- I forgot the actual registers used here
	// mov rax, [rbx + 0x100]
	// Which means "dereference a pointer to an object on offset 0x568, then dereference a pointer to an int on 0x100 offset of that object"
	inline int GetCastRange() {
		auto infoObj = Member<VClass*>(0x568);
		return *infoObj->Member<int*>(0x100); // Weird structure
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
		return Interfaces::EntitySystem->GetEntity<BaseNpc>(H2IDX(GetAssignedHeroHandle()));
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
			H2IDX(handle),
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
extern std::vector<BaseEntity*> physicalItems; // items dropped onto the floor