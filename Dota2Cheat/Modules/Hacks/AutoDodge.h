#pragma once

namespace Config {
#ifdef _DEBUG
	inline bool AutoDodge = true;
#else
	inline bool AutoDodge = false;
#endif // _DEBUG
}

namespace Hacks {
	class AutoDodge {
	public:
		void FrameBasedLogic();
	};
}
namespace Modules {
	inline Hacks::AutoDodge AutoDodge{};
}