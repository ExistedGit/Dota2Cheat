#pragma once
#include "CheatSDK/Hooking.h"

#include "DebugFunctions.h"

#include "Utils/Drawing.h"

#include "CheatSDK/include.h"
#include "CheatSDK/MatchStateHandling.h"

#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"

//static void* oSpawn{};
//void* hkSpawn(CDOTABaseNPC* hero) {
//	decltype(&hkSpawn) OSpawn = (decltype(&hkSpawn))oSpawn;
//	if (!DoChanging || !EntityList.IsHero(hero))
//		return OSpawn(hero);
//
//
//	auto& w = hero->Field<CUtlVector<CHandle<CEconWearable>>>(Netvars::C_BaseCombatCharacter::m_hMyWearables);
//	Log(LP_NONE, "WEARABLES:");
//	for (int i = 0; i < w.m_Size; i++) {
//		LogF(LP_NONE, "{} | {}", i, (void*)w[i].Entity());
//	}
//
//	static Function init = Memory::Scan("E8 ? ? ? ? 8B C6 3B F3", "client.dll").GetAbsoluteAddress(1);
//	static auto ei = Modules::SkinChanger.CreateItem(18033);
//	auto item = hero->Wearables()[2]->GetAttributeManager()->GetItem();
//	init(item, ei, 0xff);
//
//	auto ret = OSpawn(hero);
//
//	return ret;
//}
static void* oOnDataChanged{};
static void* oSpawn{};
void* hkOnDataChanged(CDOTABaseNPC* hero, bool idk) {
	decltype(&hkOnDataChanged) OOnDataChanged = (decltype(&hkOnDataChanged))oOnDataChanged;
	if (idk != 0 || !EntityList.IsHero(hero))
		return OOnDataChanged(hero, idk);

	Log(LP_NONE, "OnDataChanged");


	static Function init = Memory::Scan("E8 ? ? ? ? 8B C6 3B F3", "client.dll").GetAbsoluteAddress(1);
	static auto ei = Modules::SkinChanger.CreateItem(9235);
	CEconWearable* wearable = hero->Wearables()[0];
	auto item = wearable->GetAttributeManager()->GetItem();

	LogF(LP_NONE, "W0 Model: {}", (wearable->GetModelName() ? wearable->GetModelName() : "N/A"));
	if (!DoChanging)
		return OOnDataChanged(hero, idk);

	init(item, ei, 0xff);


	//hero->Field<bool>(0x16d8) = 1;

	// static Function setModel = Memory::Scan("E8 ? ? ? ? 8B 7D 6F", "client.dll").GetAbsoluteAddress(1);
	// static Function wInit = Address(wearable->GetVFunc(7).ptr).Offset(0x18).GetAbsoluteAddress(1);
	/// setModel(wearable, ei->GetItemDef()->CallVFunc<8, const char*>(0));
	// wInit(hero->Wearables()[2]);

	hero->Field<bool>(0x1075) = true;

	auto ret = OOnDataChanged(hero, idk);
	return ret;
}

void* (*oCtor)(VClass*) = {};
void* hkCtor(VClass* w) {
	auto ret = oCtor(w);
	if (!DoChanging) {
		Log(LP_NONE, "W0: ", (void*)w);
		Sleep(10000);
	}
	return ret;
}

void* oESCrap{};
void* hkESCrap(CBaseEntity::CSkeletonInstance* thisptr, uint32_t idk, bool* res) {
	auto str = thisptr->GetOwner()->SchemaBinding()->binaryName;
	if (str && !strcmp(str, "C_DOTAWearableItem")) {
		return (decltype(&hkESCrap)(oESCrap))(thisptr, idk, res);
	}
	return (decltype(&hkESCrap)(oESCrap))(thisptr, idk, res);
}

//static void(*oSetUpDemoHeroItems)(VClass*) = {};
//void hkSetUpDemoHeroItems(VClass* invMgr) {
//	auto view = invMgr->MemberInline<CEconItemView>(0x558 + 0xC0 * 0);
//
//	static Function init = Memory::Scan("E8 ? ? ? ? 8B C6 3B F3", "client.dll").GetAbsoluteAddress(1);
//	static auto ei = Modules::SkinChanger.CreateItem(8593);
//	// auto item = ent->Wearables()[1]->GetAttributeManager()->GetItem();
//	auto& vec = invMgr->Field<CUtlVector<uint64_t>>(0x1d58);
//	if (vec.m_Size == 1)
//		vec[0] = ei->m_ulID;
//	
//	// init(view, ei, 0xFF);
//	oSetUpDemoHeroItems(invMgr);
//}

struct shit {
	CBaseEntity* ent;
	PAD(0X30);
	const char** netvarDef;
};

void* oDecode{};
bool hkDecode(CFlattenedSerializer* thisptr, shit* s, void* idk, void* idk2) {
	if (s->ent == ctx.localHero)
		writes[*s->netvarDef]++;
	return ((decltype(&hkDecode))(oDecode))(thisptr, s, idk, idk2);
}
void* oIdk{};
void* hkIdk(void* u1, void* u2, void* u3, void* u4, void* u5) {
	return oIdk ? ((decltype(&hkIdk))(oIdk))(u1, u2, u3, u4, u5) : nullptr;
}

uintptr_t WINAPI HackThread(HMODULE hModule) {
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	Modules::SkinChanger.DeleteSOCacheFiles();

	Log(LP_INFO, "Initializing cheat...");
	d2c.Initialize(hModule);

	//{
	//	auto Spawn = Memory::Scan("48 89 5C 24 ? 48 89 54 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 83 3D", "client.dll");
	//	HOOKFUNC(Spawn);
	//}
	//{
	//	auto Ctor = Memory::Scan("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 03 48 8D 05 ? ? ? ? 48 89 83 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 83 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 83 ? ? ? ? 33 C0 C7 83", "client.dll");
	//	HOOKFUNC(Ctor);
	//}

	//{
	//	auto ESCrap = Memory::Scan("48 8B C4 4C 89 40 18 89 50 10 48 89 48 08 55 57", "client.dll");
	//	HOOKFUNC(ESCrap);
	//}
	//{
	//	auto OnDataChanged = Memory::Scan("E8 ? ? ? ? 85 DB 75 34", "client.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(OnDataChanged);
	//}

	//{
	//	auto Decode = Memory::Scan("E8 ? ? ? ? E9 ? ? ? ? 8B CA", "networksystem.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(Decode);
	//}
	
	auto& nc = Interfaces::FlattenedSerializers->NetworkedClasses();
	for (auto& c : nc) {
		if (!c.className || !std::string_view(c.className).starts_with("client.dll"))
			continue;
		auto& nv = c.c->s2->NetVars();
		for (auto& v : nv) {
			if (strcmp(v->name, "m_iItemDefinitionIndex"))
				continue;

			auto& Idk = v->callback;
			if (Idk)
				HOOKFUNC(Idk);
			else
				Idk = hkIdk;
			break;
		}
		if (oIdk)
			break;
	}

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
		if (HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0); thread)
			CloseHandle(thread);

	return TRUE;
}