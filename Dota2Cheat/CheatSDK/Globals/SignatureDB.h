#pragma once
#include <json.hpp>
#include <map>
#include <string>
#include "../../SDK/Base/Memory.h"

// Used to process JSON signatures

#define SIGMAP_ENTRY(var) {#var, (void**)&var}
class SignatureDB {
	inline static nlohmann::json Data;
public:
	static Address FindSignature(std::string_view sigName) {
		enum SignatureAction {
			GetAbsoluteAddress,
			Offset
		};

		if (!Data.contains(sigName))
			return;

		auto& info = Data[sigName];
		std::string sigStr = info["signature"], sigModule = info["module"];

		auto res = Memory::Scan(sigStr, sigModule);
		if (!res)
			return;

		if (info.contains("steps")) {
			for (auto& pair : info["steps"].items()) {
				SignatureAction type = pair.value()[0];
				int value = pair.value()[1];
				switch (type) {
				case GetAbsoluteAddress: res = res.GetAbsoluteAddress(value); break;
				case Offset: res = res.Offset(value); break;
				}
			}
		}
		return res;
	}

	// Use a map of signature names to variable pointers to perform a sigscan
	static void ParseSignatures(const std::map <std::string, void**>& signatureMap) {
		for (auto& [sigName, sigVar] : signatureMap) {
			*sigVar = FindSignature(sigName);
		}

		bool brokenSig = false;
		for (auto& [sigName, sigVar] : signatureMap) {
			LogF(*sigVar ? LP_DATA : LP_ERROR, "{}: {}", sigName);
			if (!(*sigVar))
				brokenSig = true;
		}
		if (brokenSig)
			system("pause");
	}

	static void LoadSignaturesFromFile(const std::string& url) {
		if (std::ifstream fin(url); fin.is_open()) {
			LogF(LP_INFO, "Loading signatures from {}\n", url);
			Data = nlohmann::json::parse(fin);
			fin.close();
		}
	};
};