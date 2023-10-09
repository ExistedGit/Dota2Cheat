#pragma once
#include <vector>
#include <string_view>
#include <string>
#include <cstdint>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <map>

inline bool TestStringFilters(const std::string_view& str, const std::vector<const char*>& filters) {
	for (auto& filter : filters)
		if (str.find(filter, 0) != -1)
			return true;

	return false;
}

