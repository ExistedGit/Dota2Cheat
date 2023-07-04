#pragma once
#include <sol/sol.hpp>
#include <filesystem>
#include <ShlObj_core.h>
#include "../CheatSDK/Systems/CheatManager.h"
using directory_iterator = std::filesystem::directory_iterator;

namespace Lua {

	// Calls a function in every module that has it
	template<typename ...Args>
	inline void CallModuleFunc(const std::string& funcName, Args&&... args) {
		d2c.lua["Modules"]["Core"]["CallModuleFunc"](funcName, std::forward<Args>(args)...);
	}

	inline void CallModuleFunc(const std::string& funcName) {
		d2c.lua["Modules"]["Core"]["CallModuleFunc"](funcName);
	}
}