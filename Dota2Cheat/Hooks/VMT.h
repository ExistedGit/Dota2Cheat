#pragma once
#include <memory>
#include "../vtabler.h"

namespace VMTs {
	inline std::unique_ptr<VMT> UIEngine = nullptr;
	inline std::unique_ptr<VMT> Engine = nullptr;
	inline std::unique_ptr<VMT> Entity = nullptr;
	inline std::unique_ptr<VMT> NetworkSystem = nullptr;
	inline std::unique_ptr<VMT> NetChannel = nullptr;
}