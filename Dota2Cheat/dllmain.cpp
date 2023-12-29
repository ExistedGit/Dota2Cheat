#pragma once
#include "CheatSDK/Hooking.h"

#include "DebugFunctions.h"

#include "Utils/Drawing.h"

#include "CheatSDK/include.h"
#include "CheatSDK/VTexDecoders/VTexParser.h"
#include "CheatSDK/MatchStateHandling.h"

#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"

//struct piss {
//	const char** netvar;
//	CBaseEntity* ent;
//	void* writeAddr;
//	PAD(0X10);
//	void* idk;
//};
//
//struct shit {
//	CBaseEntity* ent;
//	PAD(16);
//	piss* p; // 0x18
//	PAD(0X10);
//	int cbIndex;
//};
#define OFUNC(name) ((decltype(&hk##name))(o##name))
//
//void* oDecode{};
//bool hkDecode(CFlattenedSerializer* thisptr, shit* s, int* val, void* idk2) {
//	if (ctx.localHero)
//		for (CBaseEntity* w : ((CDOTABaseNPC_Hero*)ctx.localHero)->Wearables()) {
//			//if (s->ent == ctx.localHero) {
//			if (w == s->p->ent) {
//				std::lock_guard<std::mutex> lk(writeMutex);
//				const char* name = *s->p->netvar;
//				writes[name].lastVal = *val;
//				writes[name].writeCount++;
//			}
//		}
//
//	return OFUNC(Decode)(thisptr, s, val, idk2);
//}

void* oOnDemoHero;
bool hkOnDemoHero(CDOTAGameRules* thisptr, int entIdx, CDOTAClientMsg_DemoHero* msg) {
	//msg->mutable_item_defs()->Set(2, 6996);
	return OFUNC(OnDemoHero)(thisptr, entIdx, msg);
}

void HackThread(HMODULE hModule) {
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	Modules::SkinChanger.DeleteSOCacheFiles();

	d2c.Initialize(hModule);
	//{
	//	auto Decode = Memory::Scan("E8 ? ? ? ? 44 0F B6 BD ? ? ? ? 44 22 F8 44 88 BD ? ? ? ? E9", "networksystem.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(Decode);
	//}
	//{
	//	auto OnDemoHero = Memory::Scan("48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 8B 05 ? ? ? ? ",
	//		"client.dll");
	//	HOOKFUNC(OnDemoHero);
	//}

	MatchStateManager.CheckForOngoingGame();

	while (!d2c.shouldUnload)
		Sleep(10);

	d2c.Unload();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		std::thread(&HackThread, hModule).detach();

	return TRUE;
}