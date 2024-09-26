#pragma once	
#include "CheatSDK/Hooking.h"

#include "CheatSDK/include.h"
#include "CheatSDK/MatchStateHandling.h"

#include "UI/Pages/MainMenu.h"
#include <dota_usercmd.pb.h>

struct CDOTAInput : NormalClass {
	GETTER(int, GetSequenceNumber, 0x3D24);
};

void* oCreateMove;
void hkCreateMove(CDOTAInput* rcx, int slot, bool should_fill_weaponselect) {
	ORIGCALL(CreateMove)(rcx, slot, should_fill_weaponselect);

	auto v120 = *(DWORD*)((uintptr_t)rcx + 0x3E98 * slot + 0x3F64);
	//auto user_cmd = (CDota2UserCmdPB*)(0x3E98 * slot + 104i64 * ((int)v120 % 150) + (uintptr_t)rcx + 0x250);
	auto user_cmd = (CDota2UserCmdPB*)((char*)rcx + 0x250 + 0x68 * (v120 % 150));
	static auto RandomFloat = Memory::GetExport<Address>("tier0.dll", "RandomFloat");
	static float* rndData = RandomFloat.Offset(0x72).GetAbsoluteAddress(3);

	auto seed = user_cmd->base().random_seed();

	seed = seed;
}

// C_DOTA_BaseNPC_Hero::Spawn(CEntityKeyValues* kv)
void* oSpawn;
void hkSpawn(CDOTABaseNPC_Hero* hero, void* kv) {
	using namespace std;
	vector<CEconWearable*> pWearables;
	auto& hWearables = hero->Wearables();
	transform(hWearables.begin(), hWearables.end(), back_inserter(pWearables), [](auto h) { return *h; });

	LogW("WEARABLES");
	for (auto pWearable : pWearables) {
		pWearable->Field<bool>(Netvars::C_DOTAWearableItem::m_bIsGeneratingEconItem) = true;

		LogF("{} = {}", 
			pWearable->GetAttributeManager()->GetItem()->Member<uint32_t>(Netvars::C_EconItemView::m_iItemDefinitionIndex), 
			pWearable->GetModelName() ? pWearable->GetModelName() : "NO MODEL");
	}

	pWearables[2]->GetAttributeManager()->GetItem()->Field<uint32_t>(Netvars::C_EconItemView::m_iItemDefinitionIndex) = 6996;
	pWearables[2]->GetAttributeManager()->GetItem()->Field<bool>(Netvars::C_EconItemView::m_bInitialized) = false;
	
	pWearables[2]->CallVFunc<6>(0);
	ORIGCALL(Spawn)(hero, kv);
	
	LogW("NEW WEARABLES");
	for (auto pWearable : pWearables) {
		LogF("{} = {}",
			pWearable->GetAttributeManager()->GetItem()->Member<uint32_t>(Netvars::C_EconItemView::m_iItemDefinitionIndex),
			pWearable->GetModelName() ? pWearable->GetModelName() : "NO MODEL");
	}
}
void HackThread(HMODULE hModule) {	
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	//Modules::SkinChanger.DeleteSOCacheFiles();

	d2c.Initialize(hModule);

	//{
	//	auto Spawn = Memory::Scan("E8 ? ? ? ? 48 8B BB ? ? ? ? B9", "client.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(Spawn);
	//}

	//{
	//	auto Decode = Memory::Scan("E8 ? ? ? ? 44 0F B6 BD ? ? ? ? 44 22 F8 44 88 BD ? ? ? ? E9", "networksystem.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(Decode);
	//}

	//{
	//	auto CreateMove = Memory::Scan("44 88 44 24 ?? 89 54 24 ?? 55 53 56 57 41 54",
	//		"client.dll");
	//	HOOKFUNC(CreateMove);
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