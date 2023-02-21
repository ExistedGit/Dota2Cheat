#pragma once
#include <cstdint>
#include "Schema.h"
#include "Signatures.h"
#include "CUtlVector.h"
#include <set>
//#include <span>
#include "Netvars.h"
#include "SDK/color.h"

#define u64 unsigned long long

struct SchemaClassBinding {
	SchemaClassBinding* parent;
	const char* binaryName; // ex: C_World
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
	CSchemaClassBinding* SchemaBinding() {
		return CallVFunc<0, CSchemaClassBinding*>();
	};

	CEntityIdentity* GetIdentity() {
		return Member<CEntityIdentity*>(0x10);
	}

	void SetColor(Color clr)
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
	ENT_HANDLE GetOwnerEntityHandle() {
		return Member<ENT_HANDLE>(Schema::Netvars["C_BaseEntity"]["m_hOwnerEntity"]);
	}

	Vector3 GetPos() {
		u64 offset = Schema::Netvars["C_BaseEntity"]["m_pGameSceneNode"];
		if (!offset)
			return Vector3::Zero;
		// The m_vecAbsOrigin netvar is actually inside a CGameSceneNode
		return Member<VClass*>(offset) // getting the node
			->Member<Vector3>(0x10); // getting the absOrigin
	}

	// In degrees from 180 to -180(on 0 it looks right)
	float GetRotation() {
		u64 offset = Schema::Netvars["C_BaseEntity"]["m_pGameSceneNode"];
		if (!offset)
			return 0;
		return Member<VClass*>(offset)
			->Member<float>(0xf4);
	}

	// Gets the point in front of the entity at the specified distance
	Vector3 GetForwardVector(int dist) {
		auto pos = GetPos();
		float rotation = GetRotation() * PI / 180;

		float sine = sinf(rotation), cosine = cosf(rotation);
		auto forwardVec = Vector3(cosine * dist, sine * dist, 0);
		return pos + forwardVec;
	}
	Vector2 GetPos2D() {
		auto vec = GetPos();
		return vec.AsVec2();
	}
	int GetMaxHealth() {
		return Member<int>(Schema::Netvars["C_BaseEntity"]["m_iMaxHealth"]);
	}
	int GetHealth() {
		return Member<int>(Schema::Netvars["C_BaseEntity"]["m_iHealth"]);
	}
	DOTATeam_t GetTeam() {
		return Member<DOTATeam_t>(Schema::Netvars["C_BaseEntity"]["m_iTeamNum"]);
	}
	INT8 GetLifeState() {
		return Member<INT8>(Schema::Netvars["C_BaseEntity"]["m_lifeState"]);
	}

	static void BindLua(sol::state& lua) {
		sol::usertype<BaseEntity> type = lua.new_usertype<BaseEntity>("BaseEntity");
		type["GetSchemaBinding"] = &BaseEntity::SchemaBinding;
		type["GetIdentity"] = &BaseEntity::GetIdentity;
		type["SetColor"] = &BaseEntity::SetColor;
		type["GetOwnerEntityHandle"] = &BaseEntity::GetOwnerEntityHandle;
		type["GetPos"] = &BaseEntity::GetPos;
		type["GetRotation"] = &BaseEntity::GetRotation;
		type["GetForwardVector"] = &BaseEntity::GetForwardVector;
		type["GetPos2D"] = &BaseEntity::GetPos2D;
		type["GetMaxHealth"] = &BaseEntity::GetMaxHealth;
		type["GetHealth"] = &BaseEntity::GetHealth;
		type["GetTeam"] = &BaseEntity::GetTeam;
		type["GetLifeState"] = &BaseEntity::GetLifeState;
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
	const char* GetName() {
		return Member<const char*>(Netvars::CDOTAModifier::m_name);
	}

	float GetDuration() {
		return Member<float>(Netvars::CDOTAModifier::m_flDuration);
	}

	float GetDieTime() {
		return Member<float>(Netvars::CDOTAModifier::m_flDieTime);
	}

	ENT_HANDLE GetCaster() {
		return Member<ENT_HANDLE>(Netvars::CDOTAModifier::m_hCaster);
	}

	ENT_HANDLE GetAbility() {
		return Member<ENT_HANDLE>(Netvars::CDOTAModifier::m_hAbility);
	}

	ENT_HANDLE GetOwner() {
		return Member<ENT_HANDLE>(Netvars::CDOTAModifier::m_hParent);
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<DotaModifier>("DotaModifier");

		type["GetName"] = &DotaModifier::GetName;
		type["GetDuration"] = &DotaModifier::GetDuration;
		type["GetDieTime"] = &DotaModifier::GetDieTime;
		type["GetCaster"] = &DotaModifier::GetCaster;
		type["GetAbility"] = &DotaModifier::GetAbility;
		type["GetOwner"] = &DotaModifier::GetOwner;
	}
};

class DotaModifierManager : public VClass {
public:
	// Returns the original CUtlVector that stores the list
	CUtlVector<DotaModifier*>* GetModifierListRaw() {
		return (CUtlVector<DotaModifier*>*)((uintptr_t)this + 0x10);
	}
	std::vector<DotaModifier*> GetModifierList() {
		auto result = std::vector<DotaModifier*>{};

		auto vecModifiers = (CUtlVector<DotaModifier*>*)((uintptr_t)this + 0x10);
		for (int i = 0; i < vecModifiers->m_Size; i++)
			result.push_back(vecModifiers->at(i));

		return result;
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<DotaModifierManager>("DotaModifierManager");
		type["GetModifierList"] = &DotaModifierManager::GetModifierList;
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
	float GetCooldown() {
		return Member<float>(0x5a8);
	}
	int GetCharges() {
		return Member<int>(0x610);
	}
	int GetManaCost() {
		return Member<int>(0x5b0);
	}

	ItemStat_t GetItemStat() {
		return Member<ItemStat_t>(0x670);
	}

	// For items like Armlet
	bool IsToggled() {
		return Member<bool>(0x59d);
	}

	//int GetCastRange() {
	//	auto pos = GetPos();
	//	return CallVFunc<0x798/8, int>(&pos, nullptr, *(uintptr_t*)this);
	//}

	// Xref "Script_GetCastRangeBonus" in x64dbg to a lea rax
	// Below there will be "hTarget" or something, above that will be a lea rcx, [XXX] with a function
	// At the end there are two calls to [rdi + 0x798] and [rdi + 0x7a0], first it gets the range, then the bonus
	// I currently could not get GetCastRange to work as a standalone vfunc
	// found via dynamical analysis
	int GetCastRangeBonus() {
		return CallVFunc<244, int>(nullptr, nullptr, nullptr);
	}

	int GetEffectiveCastRange() {
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
	int GetCastRange() {
		auto infoObj = Member<VClass*>(0x568);
		return *infoObj->Member<int*>(0x100); // Weird structure
	}
	template<typename T = double>
	T GetLevelSpecialValueFor(const char* valName, int level = -1) {
		return (T)Signatures::Scripts::GetLevelSpecialValueFor(nullptr, H2IDX(GetIdentity()->entHandle), valName, level);
	}

	int GetAOERadius() {
		return static_cast<int>(Signatures::Scripts::GetLevelSpecialValueFor(nullptr, H2IDX(GetIdentity()->entHandle), "radius", -1));
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
		bool IsValid() {
			return handle != 0xFFFFFFFF;
		}

		static void BindLua(sol::state& lua) {
			auto type = lua.new_usertype<ItemOrAbility>("ItemOrAbility", sol::constructors<ItemOrAbility(const char*, ENT_HANDLE)>());

			type["GetEntity"] = &ItemOrAbility::GetEntity;
			type["GetAsAbility"] = &ItemOrAbility::GetAs<BaseAbility>;
			type["IsValid"] = &ItemOrAbility::IsValid;
		}
	};

	DotaModifierManager* GetModifierManager() {
		// Inlined into the object instead of a pointer
		return (DotaModifierManager*)((uintptr_t)this + Schema::Netvars["C_DOTA_BaseNPC"]["m_ModifierManager"]);
	}

	// Wrapper function combining the following conditions: 
	// Is not dormant
	// Is alive
	// Is not waiting to spawn
	bool IsTargetable() {
		return !GetIdentity()->IsDormant() && GetLifeState() == 0 && !IsWaitingToSpawn();
	}
	bool IsWaitingToSpawn() {
		return Member<bool>(Schema::Netvars["C_DOTA_BaseNPC"]["m_bIsWaitingToSpawn"]);
	}
	bool IsAncient() {
		return Member<bool>(Schema::Netvars["C_DOTA_BaseNPC"]["m_bIsAncient"]);
	}

	//Implemented as a method returning a bool rather than a field
	//Is inside some kind of structure on offset BE8
	bool IsRoshan() {
		return reinterpret_cast<VClass*>((uintptr_t)this + 0xbe8)->CallVFunc<57, bool>();
	}

	int GetAttackRange() {
		return Member<int>(Schema::Netvars["C_DOTA_BaseNPC"]["m_iAttackRange"]);
	}

	float GetSSC() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flStartSequenceCycle"]);
	}
	ENT_HANDLE GoalEntity() {
		return Member<ENT_HANDLE>(Schema::Netvars["C_DOTA_BaseNPC"]["m_hGoalEntity"]);
	}
	const char* GetUnitName() {
		//return *(const char**)((uintptr_t)this + Schema::Netvars["C_DOTA_BaseNPC"]["m_iszUnitName"]);
		//std::cout << std::hex << Schema::Netvars["C_DOTA_BaseNPC"]["m_iszUnitName"] << '\n';
		//std::cout << "this: " << this << '\n';
		return Member<const char*>(Schema::Netvars["C_DOTA_BaseNPC"]["m_iszUnitName"]);
	}
	std::vector<ItemOrAbility> GetAbilities() {
		static int offset = Schema::Netvars["C_DOTA_BaseNPC"]["m_hAbilities"];
		if (!offset)
			return {};

		std::vector<ItemOrAbility> result{};
		ENT_HANDLE* hAbilities = (ENT_HANDLE*)((uintptr_t)this + offset);
		for (int j = 0; j < 35; j++) {
			ENT_HANDLE handle = hAbilities[j];
			if (HVALID(handle)) {
				auto* identity = Interfaces::EntitySystem->GetIdentity(H2IDX(handle));
				result.push_back(ItemOrAbility((identity->entityName ? identity->entityName : identity->internalName), identity->entHandle));
			}
		}
		return result;
	}

	ItemOrAbility FindItemBySubstring(const char* str) {
		if (str == nullptr)
			return ItemOrAbility{ nullptr, 0xFFFFFFFF };
		for (const auto& item : GetItems())
			if (item.name &&
				strstr(item.name, str))
				return item;

		return ItemOrAbility{ nullptr, 0xFFFFFFFF };
	}

	CUnitInventory* GetInventory() {
		static int offset = Schema::Netvars["C_DOTA_BaseNPC"]["m_Inventory"];
		if (!offset)
			return nullptr;
		return (CUnitInventory*)((uintptr_t)this + offset);
	}
	std::vector<ItemOrAbility> GetItems() {
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
					result.push_back(ItemOrAbility(identity->GetName(), identity->entHandle));
				}
			}
		}
		return result;
	}

	float GetMana() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flMana"]);
	}
	float GetMaxMana() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flMaxMana"]);
	}

	float GetBaseAttackTime() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flBaseAttackTime"]);
	}
	float GetHullRadius() {
		return Member<float>(Schema::Netvars["C_DOTA_BaseNPC"]["m_flHullRadius"]);
	}
	float GetAttackSpeed() {
		return 1 + CallVFunc<295, float>();
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<BaseNpc>("BaseNpc", sol::base_classes, sol::bases<BaseEntity>());

		type["GetModifierManager"] = &BaseNpc::GetModifierManager;
		type["IsTargetable"] = &BaseNpc::IsTargetable;
		type["IsWaitingToSpawn"] = &BaseNpc::IsWaitingToSpawn;
		type["IsAncient"] = &BaseNpc::IsAncient;
		type["IsRoshan"] = &BaseNpc::IsRoshan;
		type["GetAttackRange"] = &BaseNpc::GetAttackRange;
		type["GetSSC"] = &BaseNpc::GetSSC;
		type["GoalEntity"] = &BaseNpc::GoalEntity;
		type["GetUnitName"] = &BaseNpc::GetUnitName;
		type["GetAbilities"] = &BaseNpc::GetAbilities;
		type["FindItemBySubstring"] = &BaseNpc::FindItemBySubstring;
		type["GetInventory"] = &BaseNpc::GetInventory;
		type["GetItems"] = &BaseNpc::GetItems;;
		type["GetMana"] = &BaseNpc::GetMana;
		type["GetMaxMana"] = &BaseNpc::GetMaxMana;
		type["GetBaseAttackTime"] = &BaseNpc::GetBaseAttackTime;
		type["GetHullRadius"] = &BaseNpc::GetHullRadius;
		type["GetAttackSpeed"] = &BaseNpc::GetAttackSpeed;
	}
};

class BaseNpcHero : public BaseNpc {
public:
	struct HeroAttributes {
		float strength, agility, intellect;
	};

	HeroAttributes GetAttributes() {
		return Member<HeroAttributes>(Schema::Netvars["C_DOTA_BaseNPC_Hero"]["m_flStrengthTotal"]);
	}

	bool IsIllusion() {
		return Member<ENT_HANDLE>(Schema::Netvars["C_DOTA_BaseNPC_Hero"]["m_hReplicatingOtherHeroModel"]) != 0xFFFFFFFF;
	}
};

class DotaPlayer : public BaseEntity {
public:
	uint32_t GetAssignedHeroHandle() {
		return Member< uint32_t>(Schema::Netvars["C_DOTAPlayerController"]["m_hAssignedHero"]);
	}
	std::vector<uint32_t> GetSelectedUnits() {
		return Member<CUtlVector<uint32_t>>(Schema::Netvars["C_DOTAPlayerController"]["m_nSelectedUnits"]).ToStdVector();
	}
	BaseNpc* GetAssignedHero() {
		return Interfaces::EntitySystem->GetEntity<BaseNpc>(H2IDX(GetAssignedHeroHandle()));
	}
	uint64_t GetSteamID() {
		return Member<uint64_t>(0x6b8);
	}
	void CastNoTarget(ENT_HANDLE handle, BaseEntity* issuer = nullptr) {
		if (issuer == nullptr)
			issuer = GetAssignedHero();
		PrepareOrder(DOTA_UNIT_ORDER_CAST_NO_TARGET,
			0,
			&Vector3::Zero,
			H2IDX(handle),
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			issuer,
			false,
			true
		);
	}
	void BuyItem(int itemId) {
		PrepareOrder(DOTA_UNIT_ORDER_PURCHASE_ITEM,
			1,
			&Vector3::Zero,
			itemId,
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			GetAssignedHero(),
			false,
			true
		);
	}
	void OrderMoveTo(Vector3* pos, bool directMovement = false, BaseEntity* issuer = nullptr) {
		if (issuer == nullptr)
			issuer = GetAssignedHero();

		PrepareOrder(directMovement ? DOTA_UNIT_ORDER_MOVE_TO_DIRECTION : DOTA_UNIT_ORDER_MOVE_TO_POSITION,
			0,
			pos,
			0,
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			issuer,
			false,
			true
		);
	}
	void PrepareOrder(DotaUnitOrder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue = false, bool showEffects = true) {
		//if (issuer == nullptr)
		//	issuer = GetAssignedHero();

		if (Signatures::PrepareUnitOrders == nullptr)
			return;

		Signatures::PrepareUnitOrders(this, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<DotaPlayer>("DotaPlayer", sol::base_classes, sol::bases<BaseEntity>());
		type["GetAssignedHeroHandle"] = &DotaPlayer::GetAssignedHeroHandle;
		type["GetSelectedUnits"] = &DotaPlayer::GetSelectedUnits;
		type["GetAssignedHero"] = &DotaPlayer::GetAssignedHero;
		type["GetSteamID"] = &DotaPlayer::GetSteamID;

		type["PrepareOrder"] = &DotaPlayer::PrepareOrder;
		type["CastNoTarget"] = &DotaPlayer::CastNoTarget;
		type["OrderMoveTo"] = &DotaPlayer::OrderMoveTo;
		type["BuyItem"] = &DotaPlayer::BuyItem;
	}
};

