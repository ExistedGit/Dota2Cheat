#pragma once
#include "../base/Definitions.h"
#include <sol/sol.hpp>

class CBaseEntity;

class CEntityIdentity {

public:
	CBaseEntity* entity;
	void* dunno;
	int entHandle; // LOWORD(handle) & 0x7FFF = entID
	int unk2; // always seems to be -1
	const char* internalName; // these two strings are optional!
	const char* entityName; // ex: item_tpscroll
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
		return internalName ? internalName : entityName;
	}

	bool IsDormant() const {
		return (flags[0] & 0x80);
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
