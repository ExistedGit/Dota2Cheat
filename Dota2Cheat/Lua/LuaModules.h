#pragma once
#include "sdk.h"
#include <filesystem>


using directory_iterator = std::filesystem::directory_iterator;

namespace Lua {
	
	inline void LoadScriptFiles(sol::state& lua) {
		lua.create_named_table("Modules");
		for (auto& file : directory_iterator(R"(C:\Users\user\Documents\Dota2Cheat\scripts)")) {
			auto path = file.path();
			if (path.string().substr(path.string().size() - 3) == "lua") {
				lua.load_file(path.string())();
			}
		}
	}

	template<typename ...Args>
	inline void CallModuleFunc(const std::string& funcName, Args&&... args) {
		sol::table luaModules = ctx.lua["Modules"];
		for (auto& pair : luaModules) {
			sol::function callback = pair.second.as<sol::table>()[funcName];
			if (callback.get_type() != sol::type::nil)
				callback(args...);
		}
	}
}