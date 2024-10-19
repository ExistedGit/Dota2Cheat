#pragma once
#include <string>
#include <ShlObj.h>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <fstream>

#include <Base/Logging.h>

// D2C's context. Loads and unloads data for the cheat.

inline class CCheatManager {
	FILE* consoleStream{};
	HMODULE hModule{};
	std::string cheatFolderPath;
public:

	bool shouldUnload = false;

	void LoadVMI();
	
	void LoadGameSpecific();
	void LoadFiles();
	
	void FindCheatFolder() {
		char buf[256];
		SHGetSpecialFolderPathA(0, buf, CSIDL_PROFILE, false);
		cheatFolderPath = buf;
		cheatFolderPath += "\\Documents\\Dota2Cheat";
	}

	std::string GetResource(const std::string& path) {
		return cheatFolderPath + "\\" + path;
	}

	void Initialize(HMODULE hModule);

	void SaveConfig();

	void Unload();
} d2c;

inline class CLocale {
	nlohmann::json data;
public:
	nlohmann::json schema;

	void Init(std::string locale = "en_US") {
		auto schemaPath = d2c.GetResource("localization\\lang_schema.json");
		std::ifstream fin(schemaPath);
		if (!fin)
			return;

		fin >> schema;

		fin.close();

		if (!schema.contains(locale)) {
			LogFE("Unknown locale {}, defaulting to en_US!", locale);

			locale = "en_US";
		}

		auto langPath = d2c.GetResource("localization\\" + locale + ".json");
		fin.open(langPath);

		if (!fin)
			return;

		fin >> data;

		fin.close();
	}

	const char* Get(const char8_t* token) {
		auto res = GetJsonEntryFromToken((const char*)token);
		if (!res)
			return nullptr;
		return std::string_view(*res).data();
	}

	nlohmann::json* GetJsonEntryFromToken(const std::string& name) {
		using json = nlohmann::json;
		const static std::string delimiter = ".";
		auto nameCopy = name;
		json* entryPtr = nullptr;
		
		size_t pos = 0;
		std::string token;
		while ((pos = nameCopy.find(delimiter)) != std::string::npos) {
			token = nameCopy.substr(0, pos);
			if (!entryPtr)
				entryPtr = &data[token];
			else
				entryPtr = &((*entryPtr)[token]);

			nameCopy.erase(0, pos + delimiter.length());
		}
		
		if (!entryPtr)
			entryPtr = &data[name];
		else
			entryPtr = &((*entryPtr)[nameCopy]);
		return entryPtr;
	}
} locale;

