#pragma once
#include "../SDK/pch.h"
#include <sol/sol.hpp>

namespace Lua {
	void InitEnums(sol::state& lua);
	void SetGlobals(sol::state& lua);
	void InitInterfaces(sol::state& lua);
	void InitClasses(sol::state& lua);
	void InitFunctions(sol::state& lua);
}