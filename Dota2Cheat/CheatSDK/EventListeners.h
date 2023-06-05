#pragma once
#include "../SDK/pch.h"
#include "../Modules/Hacks/AegisSnatcher.h"
#include <format>

class EntityEventListener : public IEntityListener {

	void OnEntityCreated(CBaseEntity* ent) override {
		SortEntToCollections(ent);

		if (ent->SchemaBinding()->binaryName && !strcmp(ent->SchemaBinding()->binaryName, "C_DOTAGamerulesProxy"))
			GameSystems::GameRules = ent->Member<CDOTAGameRules*>(Netvars::C_DOTAGamerulesProxy::m_pGameRules);
	}

	void OnEntityDeleted(CBaseEntity* ent) override {
		if (!ent->SchemaBinding()->binaryName)
			return;

		ctx.physicalItems.erase(ent);
		ctx.heroes.erase((CDOTABaseNPC_Hero*)ent);
		ctx.creeps.erase((CDOTABaseNPC*)ent);
		ctx.entities.erase(ent);
		ctx.runes.erase((CDOTAItemRune*)ent);

		Modules::AegisSnatcher.RemoveIfAegis(ent);
	}

	void OnEntitySpawned(CBaseEntity* ent) override { }
	void OnEntityParentChanged(CBaseEntity* ent, CBaseEntity* parent) override { };
};

class RoshanListener : public IGameEventListener2 {
public:
	int gameStartTime = 0;
	void DESTROY() override {

	}
	void FireGameEvent(CGameEvent* ev) override {
#ifdef _DEBUG
		if (gameStartTime <= 4)
			gameStartTime = 0;

		int gameTime = GameSystems::GameRules->GetGameTime();

		int offset = 0;
		int respTimeMin = 0, respTimeMax = 0;

		switch (GameSystems::GameRules->GetGameMode()) {
		case DOTA_GAMEMODE_TURBO: offset = 60; respTimeMin = 5; respTimeMax = 6; break;
		case DOTA_GAMEMODE_AP: case DOTA_GAMEMODE_ALL_DRAFT: offset = 90; respTimeMin = 8; respTimeMax = 11; break;
		}

		int minutes = (gameTime - gameStartTime - offset) / 60,
			seconds = (gameTime - gameStartTime - offset) % 60;



		//Interfaces::InputService->CmdCommand(std::format("say_team Roshan died: {}{}:{}{}",
		//	(minutes < 10 ? "0" : ""),
		//	minutes,
		//	(seconds < 10 ? "0" : ""),
		//	seconds).c_str());
		Interfaces::InputService->CmdCommand(std::format("say_team Roshan respawn: {}{}:{}{}-{}{}:{}{}",

			(minutes + respTimeMin < 10 ? "0" : ""),
			minutes + respTimeMin,
			(seconds < 10 ? "0" : ""),
			seconds,

			(minutes + respTimeMax < 10 ? "0" : ""),
			minutes + respTimeMax,
			(seconds < 10 ? "0" : ""),
			seconds

		).c_str());
		std::cout << std::dec << "Roshan died: "
			<< (minutes < 10 ? "0" : "") << minutes << ':'
			<< (seconds < 10 ? "0" : "") << seconds << "\n";
#endif // _DEBUG
	}
};