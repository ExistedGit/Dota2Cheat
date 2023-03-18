#pragma once
#include <sol/sol.hpp>
#include <filesystem>
#include <ShlObj_core.h>


using directory_iterator = std::filesystem::directory_iterator;

namespace Lua {
	
	// Loads and executes scripts from the cheat's folder in C:\%USER%\Documents\Dota2Cheat\scripts
	inline void LoadScriptFiles(sol::state& lua) {

		lua.create_named_table("Modules");
		for (auto& file : directory_iterator(ctx.cheatFolderPath + R"(\scripts)")) {
			auto path = file.path();
			if (path.string().substr(path.string().size() - 3) == "lua")
				lua.load_file(path.string())();
		}
	}

	// Calls a function in every module that has it
	template<typename ...Args>
	inline void CallModuleFunc(const std::string& funcName, Args&&... args) {
		ctx.lua["Modules"]["Core"]["CallModuleFunc"](funcName, args...);
	}

	inline void CallModuleFunc(const std::string& funcName) {
		ctx.lua["Modules"]["Core"]["CallModuleFunc"](funcName);
	}
}