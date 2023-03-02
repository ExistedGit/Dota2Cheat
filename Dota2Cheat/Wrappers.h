#pragma once
#include <cstdint>
#include "Schema.h"
#include "Signatures.h"
#include "CUtlVector.h"
#include <set>
#include <span>
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

class CUnitInventory : NormalClass {
public:
	// Returns an array of 19 entity handles representing slots, if the slot is empty, the handle is invalid(0XFFFFFFFF)
	// Valid handles of items are ordered by slots, i. e. moving an item to backpack will change its index inside this array
	auto GetItems() {
		return std::span<ENT_HANDLE, 19>{ MemberInline<ENT_HANDLE>(Netvars::C_DOTA_UnitInventory::m_hItems), 19 };
	}
	bool IsItemInSlot(ENT_HANDLE item, uint32_t slot) {
		if (slot > 18)
			return false;

		return GetItems()[slot] == item;
	}
	int GetItemSlot(ENT_HANDLE item) {
		auto items = GetItems();
		auto it = std::find(items.begin(), items.end(), item);
		return it != items.end()
			? it - items.begin() 
			: -1;
	}
};


class BaseEntity : public VClass {
public:
	CSchemaClassBinding* SchemaBinding() {
		return CallVFunc<0, CSchemaClassBinding*>();
	};

	CEntityIdentity* GetIdentity() {
		return Member<CEntityIdentity*>(Netvars::CEntityInstance::m_pEntity);
	}

	void SetColor(Color clr)
	{
		uintptr_t clrAddr = (uintptr_t)this + Netvars::C_BaseModelEntity::m_clrRender;
		*(BYTE*)(clrAddr + 0) = clr.RGBA[0];
		*(BYTE*)(clrAddr + 1) = clr.RGBA[1];
		*(BYTE*)(clrAddr + 2) = clr.RGBA[2];
		*(BYTE*)(clrAddr + 3) = clr.RGBA[3];

		Signatures::OnColorChanged(this);
	}
	ENT_HANDLE GetOwnerEntityHandle() {
		return Member<ENT_HANDLE>(Netvars::C_BaseEntity::m_hOwnerEntity);
	}

	Vector3 GetPos() {
		// The m_vecAbsOrigin netvar is actually inside a CGameSceneNode
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode) // getting the node
			->Member<Vector3>(Netvars::CGameSceneNode::m_vecAbsOrigin); // getting the absOrigin
	}

	// In degrees from 180 to -180(on 0 it looks right)
	float GetRotation() {
		return Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)
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
		return Member<int>(Netvars::C_BaseEntity::m_iMaxHealth);
	}
	int GetHealth() {
		return Member<int>(Netvars::C_BaseEntity::m_iHealth);
	}
	DOTA_GC_TEAM GetTeam() {
		return Member<DOTA_GC_TEAM>(Netvars::C_BaseEntity::m_iTeamNum);
	}
	INT8 GetLifeState() {
		return Member<INT8>(Netvars::C_BaseEntity::m_lifeState);
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
		return Member<const char*>(Netvars::CDOTA_Buff::m_name);
	}

	float GetDuration() {
		return Member<float>(Netvars::CDOTA_Buff::m_flDuration);
	}

	float GetDieTime() {
		return Member<float>(Netvars::CDOTA_Buff::m_flDieTime);
	}

	ENT_HANDLE GetCaster() {
		return Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hCaster);
	}

	ENT_HANDLE GetAbility() {
		return Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hAbility);
	}

	BaseNpc* GetOwner() {
		return Interfaces::EntitySystem->GetEntity<BaseNpc>(
			H2IDX(
				Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hParent)
			)
			);
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
	auto GetModifierListRaw() {
		return MemberInline<CUtlVector<DotaModifier*>>(0x10);
	}
	auto GetModifierList() {
		auto vecModifiers = MemberInline<CUtlVector<DotaModifier*>>(0x10);
		return vecModifiers->AsStdVector();
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
		return Member<float>(Netvars::C_DOTABaseAbility::m_fCooldown);
	}
	int GetCharges() {
		return Member<int>(Netvars::C_DOTABaseAbility::m_nAbilityCurrentCharges);
	}
	int GetManaCost() {
		return Member<int>(Netvars::C_DOTABaseAbility::m_iManaCost);
	}

	// For PT and Vambrace
	ItemStat_t GetItemStat() {
		return Member<ItemStat_t>(0x670);
	}

	// For items like Armlet
	bool IsToggled() {
		return Member<bool>(Netvars::C_DOTABaseAbility::m_bToggleState);
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
		return *infoObj->Member<int*>(0x100); // Weird key-value structure
	}
	template<typename T = double>
	T GetLevelSpecialValueFor(const char* valName, int level = -1) {
		return (T)Signatures::Scripts::GetLevelSpecialValueFor(nullptr, H2IDX(GetIdentity()->entHandle), valName, level);
	}

	int GetAOERadius() {
		return static_cast<int>(Signatures::Scripts::GetLevelSpecialValueFor(nullptr, H2IDX(GetIdentity()->entHandle), "radius", -1));
	}

	bool IsHidden() {
		return Member<bool>(Netvars::C_DOTABaseAbility::m_bHidden);
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
		bool IsValid() const {
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
		return MemberInline<DotaModifierManager>(Netvars::C_DOTA_BaseNPC::m_ModifierManager);
	}

	// Wrapper function combining the following conditions: 
	// Is not dormant
	// Is alive
	// Is not waiting to spawn
	bool IsTargetable() {
		return !GetIdentity()->IsDormant() && GetLifeState() == 0 && !IsWaitingToSpawn();
	}
	bool IsWaitingToSpawn() {
		return Member<bool>(Netvars::C_DOTA_BaseNPC::m_bIsWaitingToSpawn);
	}
	bool IsAncient() {
		return Member<bool>(Netvars::C_DOTA_BaseNPC::m_bIsAncient);
	}

	//Implemented as a method returning a bool rather than a field
	//Is inside some kind of structure on offset BE8
	bool IsRoshan() {
		return MemberInline<VClass>(0xbe8)->CallVFunc<57, bool>();
	}

	int GetAttackRange() {
		return Member<int>(Netvars::C_DOTA_BaseNPC::m_iAttackRange);
	}

	float GetSSC() {
		return Member<float>(Netvars::C_DOTA_BaseNPC::m_flStartSequenceCycle);
	}
	ENT_HANDLE GoalEntity() {
		return Member<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC::m_hGoalEntity);
	}
	const char* GetUnitName() {
		//return *(const char**)((uintptr_t)this + Netvars::C_DOTA_BaseNPC::m_iszUnitName);
		//std::cout << std::hex << Netvars::C_DOTA_BaseNPC::m_iszUnitName << '\n';
		//std::cout << "this: " << this << '\n';
		return Member<const char*>(Netvars::C_DOTA_BaseNPC::m_iszUnitName);
	}
	std::vector<ItemOrAbility> GetAbilities() {
		std::vector<ItemOrAbility> result{};
		ENT_HANDLE* hAbilities = MemberInline<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC::m_hAbilities);
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
		if (!str)
			return ItemOrAbility{ nullptr, 0xFFFFFFFF };
		for (const auto& item : GetItems())
			if (item.name &&
				strstr(item.name, str))
				return item;

		return ItemOrAbility{ nullptr, 0xFFFFFFFF };
	}

	CUnitInventory* GetInventory() {
		return MemberInline<CUnitInventory>(Netvars::C_DOTA_BaseNPC::m_Inventory);
	}
	std::vector<ItemOrAbility> GetItems() {
		std::vector<ItemOrAbility> result{};
		CUnitInventory* inv = GetInventory();
		if (!inv)
			return result;

		auto itemsHandle = inv->GetItems();
		for(auto& handle : itemsHandle) { 
			if (!HVALID(handle))
				continue;

			auto* identity = Interfaces::EntitySystem->GetIdentity(H2IDX(handle));
			result.push_back(ItemOrAbility(identity->GetName(), identity->entHandle));
		}

		return result;
	}

	float GetMana() {
		return Member<float>(Netvars::C_DOTA_BaseNPC::m_flMana);
	}
	float GetMaxMana() {
		return Member<float>(Netvars::C_DOTA_BaseNPC::m_flMaxMana);
	}

	float GetBaseAttackTime() {
		return Member<float>(Netvars::C_DOTA_BaseNPC::m_flBaseAttackTime);
	}
	float GetHullRadius() {
		return Member<float>(Netvars::C_DOTA_BaseNPC::m_flHullRadius);
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
		lua["BaseNpc"] = [](BaseEntity* ent) { return (BaseNpc*)ent; };
	}
};

class BaseNpcHero : public BaseNpc {
public:
	struct HeroAttributes {
		float strength, agility, intellect;
	};

	HeroAttributes GetAttributes() {
		return Member<HeroAttributes>(Netvars::C_DOTA_BaseNPC_Hero::m_flStrengthTotal);
	}

	bool IsIllusion() {
		return HVALID(Member<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC_Hero::m_hReplicatingOtherHeroModel));
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<BaseNpcHero>(
			"BaseNpcHero",
			sol::base_classes, sol::bases<BaseNpc, BaseEntity>()
			);
		type["GetAttributes"] = &BaseNpcHero::GetAttributes;
		type["IsIllusion"] = &BaseNpcHero::IsIllusion;
		lua["BaseNpcHero"] = sol::overload(
			[](BaseNpc* npc) {return (BaseNpcHero*)npc; },
			[](BaseEntity* ent) {return (BaseNpcHero*)ent; }
		);
	}
};

class DotaPlayer : public BaseEntity {
public:
	uint32_t GetAssignedHeroHandle() {
		return Member< uint32_t>(Netvars::C_DOTAPlayerController::m_hAssignedHero);
	}
	auto GetSelectedUnits() {
		return Member<CUtlVector<uint32_t>>(Netvars::C_DOTAPlayerController::m_nSelectedUnits).AsStdVector();
	}
	BaseNpc* GetAssignedHero() {
		return Interfaces::EntitySystem->GetEntity<BaseNpc>(H2IDX(GetAssignedHeroHandle()));
	}
	uint64_t GetSteamID() {
		return Member<uint64_t>(Netvars::CBasePlayerController::m_steamID);
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
	void PrepareOrder(dotaunitorder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue = false, bool showEffects = true) {
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

