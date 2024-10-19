#pragma once
#include <Base/Memory.h>
#include <nlohmann/json.hpp>
#include <map>
#include <string>

// Used to process JSON signatures

class SignatureDB {
	struct Signature {
		enum Action {
			GetAbsoluteAddress = 0,
			Offset = 1
		};

		struct Step {
			Action action;
			uint32_t parameter;
		};

		std::string data; // binary form
		std::string dll;
		std::vector<Step> steps;
	};

	inline static std::unordered_map<std::string, Signature> Data;
public:
	static Address FindSignature(const std::string& sigName);

	// Use a map of signature names to variable pointers to perform a sigscan
	static void ParseSignatures(const std::map <std::string, void**>& signatureMap);

	static void LoadSignaturesFromFile(const std::string& url);
};