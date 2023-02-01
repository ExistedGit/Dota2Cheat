#pragma once
#include <vector>
#include <iostream>

class IGameEventListener2
{
	virtual void DESTROY() = 0;
	//virtual void DESTROY2() = 0;

	virtual void FireGameEvent(void* event) = 0;
};

class CGameEventManager
{
public:
	static std::vector<std::unique_ptr<IGameEventListener2>> EventListeners;
	virtual void DESTROY() = 0;
	//virtual void DESTROY2() = 0; AddListener is at 3, maybe something got removed?
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset(void) = 0;
	virtual bool AddListener(IGameEventListener2* listener, const char* eventName, bool serverSide) = 0;
	virtual bool FindListener(IGameEventListener2* listener, const char* eventName) = 0;
	virtual void RemoveListener(IGameEventListener2* listener) = 0;
};
