#pragma once
#include <mutex>

#define MTM_LOCK std::lock_guard<std::mutex> _lock(this->_mData);

// UI modules operate their own data within different threads
// Thus we must synchronize them
class MultiThreadModule {
protected:
	std::mutex _mData;
};