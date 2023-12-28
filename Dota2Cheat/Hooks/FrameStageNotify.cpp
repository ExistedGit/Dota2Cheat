#include "FrameStageNotify.h"
#include "../Modules/UI/AbilityESP.h"
#include "../Modules/Hacks/IllusionColoring.h"
#include "../Modules/Hacks/AegisSnatcher.h"
#include "../Modules/Hacks/AutoBuyTome.h"
#include "../Modules/Hacks/RiverPaint.h"
#include "../Modules/Hacks/AutoHeal.h"
#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/AutoPing.h"
#include "../Modules/Hacks/AutoDodge.h"
#include "../Modules/Hacks/AutoMidas.h"
#include "../Modules/Hacks/DotaPlusUnlocker.h"
#include "../Modules/Hacks/SkinChanger.h"
#include "../Modules/Hacks/TreeChanger.h"

#include "../Modules/UI/UIOverhaul.h"
#include "../Modules/UI/AbilityESP.h"
#include "../Modules/UI/ParticleMaphack.h"

#include "../Modules/Utility/ParticleGC.h"

#include "../SDK/Entities/CDOTAItemRune.h"

#include "../CheatSDK/VTexDecoders/VTexParser.h"
using namespace Modules;

void UpdateCameraDistance() {
	static auto varInfo = CCVar::CVars["dota_camera_distance"];
	if (Config::CameraDistance != varInfo.m_pVar->value.flt) {
		varInfo.m_pVar->value.flt = Config::CameraDistance;
		Interfaces::CVar->TriggerCallback(varInfo);
	}
}

void UpdateWeather() {
	static auto varInfo = CCVar::CVars["cl_weather"];
	varInfo.m_pVar->value.i32 = Config::Changer::WeatherListIdx;
}


void InGameLogic() {
	//UIOverhaul.Update();

	UpdateCameraDistance();
	UpdateWeather();
	TreeChanger.UpdateTreeModels();

	static IFrameListener* PassiveListeners[] = {
		&RiverPaint,
		&IllusionESP,
		&UIOverhaul,
	};

	for (auto l : PassiveListeners)
		l->OnFrame();

	if (!GameSystems::GameRules->IsGamePaused()) {
		static IFrameListener* ActiveListeners[] = {
			&TPTracker,
			&BlinkRevealer,
			&ParticleMaphack,
			&LinearProjectileWarner,
			&ParticleGC,
			&TargetedSpellHighlighter,
		};

		static IFrameListener* HeroListeners[] = {
			&AutoHeal,
			&AutoPing,
			&AutoDodge,
			&AutoMidas,
			&AegisSnatcher,
		};


		for (auto l : ActiveListeners)
			l->OnFrame();

		if (ctx.localHero->GetLifeState() == 0) 
			for (auto l : HeroListeners)
				l->OnFrame();
		
#ifdef _DEBUG
		if (IsKeyPressed(VK_NUMPAD8)) {
			auto& w = ctx.localHero->Wearables();
			Log(LP_NONE, "Wearables:");
			for (CEconWearable* h : w) {
				Log(LP_NONE, (void*)h);
			}
			static Function apply = Memory::Scan("41 55 48 83 EC 70 4C 8B E9", "client.dll");
			apply(ctx.localHero);
		}
#endif 
	}
}

void Hooks::hkFrameStageNotify(void* thisptr, int stage) {
	((decltype(&hkFrameStageNotify))oFrameStageNotify)(thisptr, stage);

	DotaPlusManager.UpdateDotaPlusStatus();

	if (SkinChanger.ItemsCreated) {
		SkinChanger.ItemsCreated = false;

		for (auto& item : SkinChanger.itemsToCreate)
			SkinChanger.AddItem(item);

		SkinChanger.itemsToCreate.clear();
	}

	//VTexParser::Load("panorama/images/spellicons/arc_warden_spark_wraith_png.vtex_c");

	// Frame stages are cycled in this order:
	// 0 4 5 6 7 1 3 9 3 3 3
	// Meaning the 9th is end of frame and we can do our tricks
	if (stage != 9 || ctx.gameStage == GameStage::NONE)
		return;

	AbilityESP.UpdateHeroData();
	EntityList.ForEach<CDOTABaseNPC_Hero>([](auto hero) {
		HeroData[hero].AbsOrigin = hero->GetPos();
	HeroData[hero].W2S = WorldToScreen(hero->GetPos());
	HeroData[hero].HealthbarW2S = WorldToScreen(hero->GetHealthBarPos());
		});


	if (Interfaces::Engine->IsInGame() &&
		ctx.localHero &&
		ctx.gameStage == GameStage::IN_GAME)
		InGameLogic();

}
