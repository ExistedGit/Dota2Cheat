#pragma once
#include <json.hpp>
#include <map>
#include <string>

// Used to process JSON signatures

class SignatureDB {
	inline static nlohmann::json Data;
public:
	static Address FindSignature(std::string_view sigName);

	// Use a map of signature names to variable pointers to perform a sigscan
	static void ParseSignatures(const std::map <std::string, void**>& signatureMap);

	static void LoadSignaturesFromFile(const std::string& url);;
};