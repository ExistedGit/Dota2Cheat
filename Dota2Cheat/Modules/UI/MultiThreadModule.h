#pragma once
#include <mutex>

#define MTM_LOCK std::lock_guard<std::mutex> lk(this->_m);
// UI modules need to be threadsafe
class MultiThreadModule {
protected:
	std::mutex _m;
};