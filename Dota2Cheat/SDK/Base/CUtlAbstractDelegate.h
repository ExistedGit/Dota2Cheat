#pragma once

class CUtlAbstractDelegate {
	void* obj;
	void* func;
public:
	template<typename C, typename R, typename... Args>
	explicit CUtlAbstractDelegate(C* obj, R(C::* method)(Args...)) {
		func = *(void**)&method;
		this->obj = (void*)obj;
	}

	explicit CUtlAbstractDelegate(void* obj, void* method) {
		func = method;
		this->obj = obj;
	}
};