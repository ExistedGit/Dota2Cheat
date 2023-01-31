#pragma once
#include <vector>
class CGameEventListener2
{
	virtual void DESTROY() = 0;
	virtual void DESTROY2() = 0;

	virtual void FireGameEvent(void* event) = 0;
};

class CGameEventManager
{
public:
	static std::vector<CGameEventListener2*> EventListeners;
	virtual void DESTROY() = 0;
	//virtual void DESTROY2() = 0; AddListener is at 3, maybe something got removed?
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset(void) = 0;
	virtual bool AddListener(CGameEventListener2* listener, const char* eventName, bool serverSide) = 0;
	virtual bool FindListener(CGameEventListener2* listener, const char* eventName) = 0;
	virtual void RemoveListener(CGameEventListener2* listener) = 0;
};