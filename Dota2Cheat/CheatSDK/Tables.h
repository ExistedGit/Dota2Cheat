#pragma once
#include <iomanip>
#include <string>
#include <vector>

namespace tables {

	struct LogPtrBinding {
		std::string name;
		void* value;

		LogPtrBinding(std::string name, void* value) : value(value) {
			this->name = std::move(name);
		}
	};

	void PrettyPrint(const std::vector<LogPtrBinding>& table);
}