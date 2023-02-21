#pragma once
#include <sol/sol.hpp>
#include "../Enums.h"
#include "../CGameEntitySystem.h"
#include "../Wrappers.h"
#define LUA_ENUM_TABLE_ENTRY(x) #x, x

namespace Lua {
	inline void InitEnums(sol::state& lua) {
		
		auto orderTable = lua.create_table_with(
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_NONE),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_MOVE_TO_POSITION),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_MOVE_TO_TARGET),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_ATTACK_MOVE),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_ATTACK_TARGET),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_POSITION),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_TARGET),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_TARGET_TREE),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_NO_TARGET),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_TOGGLE),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_HOLD_POSITION),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_TRAIN_ABILITY),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_DROP_ITEM),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_GIVE_ITEM),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_PICKUP_ITEM),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_PICKUP_RUNE),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_PURCHASE_ITEM),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_SELL_ITEM),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_DISASSEMBLE_ITEM),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_MOVE_ITEM),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_TOGGLE_AUTO),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_STOP),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_TAUNT),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_BUYBACK),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_GLYPH),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_EJECT_ITEM_FROM_STASH),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_RUNE),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_PING_ABILITY),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_MOVE_TO_DIRECTION),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_PATROL),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_VECTOR_TARGET_POSITION),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_RADAR),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_SET_ITEM_COMBINE_LOCK),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CONTINUE),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_VECTOR_TARGET_CANCELED),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_CAST_RIVER_PAINT),
			LUA_ENUM_TABLE_ENTRY(DOTA_UNIT_ORDER_PREGAME_ADJUST_ITEM_ASSIGNMENT)
		);
		lua.create_named_table("Enum",
			"DotaUnitOrder", orderTable
		);
	}
	inline void InitInterfaces(sol::state& lua) {
		lua["EntitySystem"] = Interfaces::EntitySystem;
	}
	inline void InitClasses(sol::state& lua) {
#pragma region Base types
		{
			lua.new_usertype<Vector3>(
				"Vector3",
				sol::constructors<Vector3(), Vector3(float, float, float)>());
			sol::usertype<Vector2> type_Vector2 = lua.new_usertype<Vector2>(
				"Vector2",
				sol::constructors<Vector2(float, float)>());
			sol::usertype<Color> type_Color = lua.new_usertype<Color>(
				"Color",
				sol::constructors<Color(unsigned char, unsigned char, unsigned char, unsigned char)>()
				);
			type_Color["RGBA"] = &Color::RGBA;
		}
#pragma endregion
#pragma region Wrappers
		sol::usertype<CSchemaClassBinding> type_SchemaBinding = lua.new_usertype<CSchemaClassBinding>("CSchemaClassBinding");
		type_SchemaBinding["parent"] = &CSchemaClassBinding::parent;
		type_SchemaBinding["binaryName"] = &CSchemaClassBinding::binaryName; // ex: C_World
		type_SchemaBinding["className"] = &CSchemaClassBinding::className; // ex: client
		type_SchemaBinding["classInfoOldSynthesized"] = &CSchemaClassBinding::classInfoOldSynthesized;
		type_SchemaBinding["classInfoN"] = &CSchemaClassBinding::classInfoN;
		type_SchemaBinding["thisModuleBindingPointer"] = &CSchemaClassBinding::thisModuleBindingPointer;
		type_SchemaBinding["pSchemaType"] = &CSchemaClassBinding::pSchemaType;

		sol::usertype<CEntityIdentity> type_entIdentity = lua.new_usertype<CEntityIdentity>("CEntityIdentity");
		type_entIdentity["entity"] = &CEntityIdentity::entity;
		type_entIdentity["dunno"] = &CEntityIdentity::dunno;
		type_entIdentity["entHandle"] = &CEntityIdentity::entHandle;
		type_entIdentity["internalName"] = &CEntityIdentity::internalName; 
		type_entIdentity["entityName"] = &CEntityIdentity::entityName;
		type_entIdentity["flags"] = &CEntityIdentity::flags;
		type_entIdentity["GetName"] = &CEntityIdentity::GetName;
		type_entIdentity["IsDormant"] = &CEntityIdentity::IsDormant;

		sol::usertype<BaseEntity> type_baseEnt = lua.new_usertype<BaseEntity>("BaseEntity");
		type_baseEnt["GetSchemaBinding"] = &BaseEntity::SchemaBinding;

		type_baseEnt["GetIdentity"] = &BaseEntity::GetIdentity;
		type_baseEnt["SetColor"] = &BaseEntity::SetColor;
		type_baseEnt["GetOwnerEntityHandle"] = &BaseEntity::GetOwnerEntityHandle;

		type_baseEnt["GetPos"] = &BaseEntity::GetPos;

		// In degrees from 180 to -180(on 0 it looks right)
		type_baseEnt["GetRotation"] = &BaseEntity::GetRotation;

		// Gets the point in front of the entity at the specified distance
		type_baseEnt["GetForwardVector"] = &BaseEntity::GetForwardVector;
		type_baseEnt["GetPos2D"] = &BaseEntity::GetPos2D;
		type_baseEnt["GetMaxHealth"] = &BaseEntity::GetMaxHealth;
		type_baseEnt["GetHealth"] = &BaseEntity::GetHealth;
		type_baseEnt["GetTeam"] = &BaseEntity::GetTeam;
		type_baseEnt["GetLifeState"] = &BaseEntity::GetLifeState;

		sol::usertype<BaseNpc> type_baseNpc = lua.new_usertype<BaseNpc>("BaseNpc", sol::base_classes, sol::bases<BaseEntity>());
		type_baseNpc["GetUnitName"] = &BaseNpc::GetUnitName;


#pragma endregion
#pragma region Interfaces
		CGameEntitySystem::BindLua(lua);
#pragma endregion
	}
}