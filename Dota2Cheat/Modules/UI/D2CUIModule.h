#pragma once
#include "../../CheatSDK/include.h"
#include "../../CheatSDK/Systems/ModuleManager.h"
#include "../MListeners.h"

class D2CUIModule : public IFrameListener, public IEntityListListener {
	static inline struct {
		std::function<bool()> inGame = [] {
			return true;
			};
		
		std::function<bool()> unpaused = [] {
			return CGameRules::Get() && !CGameRules::Get()->IsGamePaused();
			};

		std::function<bool()> localHeroAlive = [this] {
			return unpaused() && ctx.localHero && ctx.localHero->IsAlive();
			};
	} standardPredicates;
public:
	virtual bool Active() const = 0;
	virtual void Draw() const = 0;

	D2CUIModule() : IFrameListener() {

		IEntityListListener::Subscribe(this);
	}
};