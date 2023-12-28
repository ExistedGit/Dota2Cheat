#pragma once
#include "../pch.h"
#include "../MatchStateHandling.h"

class event_func : public IGameEventListener2 {
public:
	std::function<void(CGameEvent*)> f;

	void SetFunc(std::function<void(CGameEvent*)> f) {
		this->f = f;
	}

	virtual void FireGameEvent(CGameEvent* ev) override
	{
		f(ev);
	}
};