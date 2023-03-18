#pragma once
#include <sol/sol.hpp>
#include "../SDK/pch.h"
#define LUA_ENUM_TABLE_ENTRY(x) #x, x

namespace Lua {
	void InitEnums(sol::state& lua);
	void SetGlobals(sol::state& lua);
	void InitInterfaces(sol::state& lua);
	void InitClasses(sol::state& lua);
	void InitFunctions(sol::state& lua);
}