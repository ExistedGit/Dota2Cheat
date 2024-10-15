#include "FrameStageNotify.h"

#include "../Modules/Hacks/AegisSnatcher.h"
#include "../Modules/Hacks/AutoBuyTome.h"
#include "../Modules/Hacks/AutoDodge.h"
#include "../Modules/Hacks/AutoHeal.h"
#include "../Modules/Hacks/AutoMidas.h"
#include "../Modules/Hacks/AutoPing.h"
#include "../Modules/Hacks/DotaPlusUnlocker.h"
#include "../Modules/Hacks/IllusionColoring.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/RiverPaint.h"
#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/SkinChanger.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/TreeChanger.h"

#include "../Modules/UI/AbilityESP/AbilityESP.h"
#include "../Modules/UI/BlinkRevealer.h"
#include "../Modules/UI/Indicators/KillIndicator.h"
#include "../Modules/UI/Indicators/SpeedIndicator.h"
#include "../Modules/UI/LastHitMarker.h"
#include "../Modules/UI/ParticleMaphack.h"
#include "../Modules/UI/TPTracker.h"
#include "../Modules/UI/UIOverhaul.h"

#include "../Modules/Utility/ParticleGC.h"

using namespace Modules;

void UpdateCameraDistance() {
	static auto& varInfo = CCVar::CVars["dota_camera_distance"];
	if (Config::CameraDistance != varInfo.m_pVar->value.flt) {
		varInfo.m_pVar->value.flt = Config::CameraDistance;
		CCVar::Get()->TriggerCallback(varInfo);
	}
}

void UpdateWeather() {
	static auto& varInfo = CCVar::CVars["cl_weather"];
	varInfo.m_pVar->value.i32 = Config::Changer::WeatherListIdx;
}

bool amodPrintScheduled = false;

void InGameLogic() {
	if (amodPrintScheduled) {
		if (ctx.localHero) {
			auto k = ctx.localHero->GetModifiedAssetString("nevermore_nev_firstblood_03");
		}

		amodPrintScheduled = false;
	}

	//UIOverhaul.Update();

	// Caching projections for later use
	EntityList.ForEach<CHero>([](auto hero) {
		HeroData[hero].W2S = WorldToScreen(hero->GetPos());
		HeroData[hero].HealthbarW2S = WorldToScreen(hero->GetHealthBarPos());
		});

	UpdateCameraDistance();
	UpdateWeather();
	TreeChanger.UpdateTreeModels();

	static IFrameListener* PassiveListeners[] = {
		&RiverPaint,
		&IllusionESP,
		&UIOverhaul,
		&AbilityESP
	};

	for (auto l : PassiveListeners)
		l->OnFrame();

	if (!CGameRules::Get()->IsGamePaused()) {
		IFrameListener* ActiveListeners[] = {
			&TPTracker,
			&BlinkRevealer,
			&ParticleMaphack,
			&LinearProjectileWarner,
			&ParticleGC,
			&TargetedSpellHighlighter,
		};

		IFrameListener* HeroListeners[] = {
			&AutoHeal,
			&AutoPing,
			&AutoDodge,
			&SpeedIndicator,
			&KillIndicator,
			&LastHitMarker,
			&AutoMidas,
			&AegisSnatcher,
		};

		for (auto l : ActiveListeners)
			l->OnFrame();

		if (ctx.localHero->GetLifeState() == 0)
			for (auto l : HeroListeners)
				l->OnFrame();
	}
}
void Hooks::hkFrameStageNotify(void* thisptr, int stage) {
	((decltype(&hkFrameStageNotify))oFrameStageNotify)(thisptr, stage);

	DotaPlusManager.UpdateDotaPlusStatus();

	// Frame stages are cycled in this order:
	// 0 4 5 6 7 1 3 9 3 3 3

	if (stage != 9 || ctx.gameStage == GameStage::NONE)
		return;

	EntityList.ForEach<CHero>(
		[](auto hero) {
			HeroData[hero].W2S = WorldToScreen(hero->GetPos());
			HeroData[hero].HealthbarW2S = WorldToScreen(hero->GetHealthBarPos());
		}
	);

	if (INetworkClientService::Get()->GetIGameClient()->IsInGame() &&
		ctx.localHero &&
		ctx.gameStage == GameStage::IN_GAME)
		InGameLogic();

}

void Hooks::FrameEventListener::OnFrameBoundary(EventFrameBoundary_t* ev) {
	Modules::DotaPlusManager.UpdateDotaPlusStatus();
	Modules::SkinChanger.ProcessCreateQueue();

	if (INetworkClientService::Get()->GetIGameClient()
		&& INetworkClientService::Get()->GetIGameClient()->IsInGame()
		&& ctx.localHero
		&& ctx.gameStage == GameStage::IN_GAME) {
		InGameLogic();
	}
}
