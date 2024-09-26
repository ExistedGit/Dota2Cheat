#include "Tables.h"
#include <Base/Logging.h>

void tables::PrettyPrint(const std::vector<LogPtrBinding>& table) {
	size_t padding = 0;
	{
		auto longestNameElem = std::max_element(table.begin(), table.end(),
			[](const LogPtrBinding& a, const LogPtrBinding& b) {
				return a.name.length() < b.name.length();
			});
		if (longestNameElem != table.end()) padding = longestNameElem->name.size();
	}

	std::lock_guard lock(mLogging);
	for (const auto& binding : table) {
		LogPrefix prefix = binding.value ? LP_DATA : LP_ERROR;
		SetConsoleColor();
		SetLogColor(prefix);

		std::cout << std::left << std::setw(padding) << binding.name << " = " << std::hex << std::uppercase << binding.value << std::endl;
	}
}
