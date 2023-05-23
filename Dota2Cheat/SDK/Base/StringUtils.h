#pragma once
#include <vector>
#include <string_view>
#include <string>
#include <cstdint>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <map>

template<typename T, typename Z>
inline bool StringsMatch(T a, Z b) {
	return !strcmp((const char*)a, (const char*)b);
}

//template <typename I>
//std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1) {
//	static const char* digits = "0123456789ABCDEF";
//	std::string rc(hex_len, '0');
//	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
//		rc[i] = digits[(uintptr_t)w >> j) & 0x0f];
//	return rc;
//}

inline std::string encodeURI(const std::string& value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;
	for (auto i = value.begin(), n = value.end(); i != n; ++i) {
		auto c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded

		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}

inline size_t WriteRemoteString(void* ptr, size_t size, size_t nmemb, void* stream) {
	std::string data((const char*)ptr, (size_t)size * nmemb);
	*((std::stringstream*)stream) << data;
	return size * nmemb;
}
inline bool TestStringFilters(const char* str, const std::vector<const char*>& filters) {
	for (auto& filter : filters)
		if (strstr(str, filter))
			return true;

	return false;
}

inline bool TestStringFilters(const std::string_view& str, const std::vector<const char*>& filters) {
	for (auto& filter : filters)
		if (str.find(filter, 0) != -1)
			return true;

	return false;
}

