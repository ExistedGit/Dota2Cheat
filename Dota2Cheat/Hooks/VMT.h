#pragma once
#include <memory>
#include "../SDK/Base/VMT.h"

namespace VMTs {
	inline std::unique_ptr<VMT> UIEngine = nullptr;
	inline std::unique_ptr<VMT> EntitySystem = nullptr;
}