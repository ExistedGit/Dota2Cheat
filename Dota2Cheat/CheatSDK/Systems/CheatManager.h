#pragma once
#include "../../SDK/pch.h"
#include <Windows.h>
#include <string>
#include <sol/sol.hpp>
#include <ShlObj.h>
// D2C's context. Loads and unloads data for the cheat.

inline class CCheatManager {
public:
	std::string cheatFolderPath;
	sol::state lua;
	FILE* consoleStream{};
	HMODULE hModule{};

	bool shouldUnload = false;

	// Loads and executes scripts from the cheat's folder in C:\Users\%USER%\Documents\Dota2Cheat\scripts
	void LoadScriptFiles();
	void LoadLua();
	void LoadGameSpecific();
	void LoadFiles();

	void FindCheatFolder() {
		char buf[256];
		SHGetSpecialFolderPathA(0, buf, CSIDL_PROFILE, false);
		cheatFolderPath = buf;
		cheatFolderPath += "\\Documents\\Dota2Cheat";
	}

	void Initialize(HMODULE hModule);

	void SaveConfig();

	void Unload();
} d2c;