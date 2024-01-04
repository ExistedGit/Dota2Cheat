#pragma once
#include "CheatSDK/Hooking.h"

#include "DebugFunctions.h"

#include "Utils/Drawing.h"

#include "CheatSDK/include.h"
#include "CheatSDK/VTexDecoders/VTexParser.h"
#include "CheatSDK/MatchStateHandling.h"

#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"
#include "SDK/Protobufs/dota_usercmd.pb.h"

enum PseudoRandom {
	DOTA_PSEUDO_RANDOM_NONE = 0,
	DOTA_PSEUDO_RANDOM_MAGNUS_SHARD = 1,
	DOTA_PSEUDO_RANDOM_PHANTOMASSASSIN_CRIT = 2,
	DOTA_PSEUDO_RANDOM_PHANTOMASSASSIN_DAGGER = 3,
	DOTA_PSEUDO_RANDOM_PHANTOMLANCER_JUXTAPOSE = 4,
	DOTA_PSEUDO_RANDOM_TINY_CRAGGY = 5,
	DOTA_PSEUDO_RANDOM_COLD_REBUKE = 6,
	DOTA_PSEUDO_RANDOM_WOLF_CRIT = 7,
	DOTA_PSEUDO_RANDOM_AXE_HELIX = 8,
	DOTA_PSEUDO_RANDOM_AXE_HELIX_ATTACK = 9,
	DOTA_PSEUDO_RANDOM_LEGION_MOMENT = 10,
	DOTA_PSEUDO_RANDOM_SLARDAR_BASH = 11,
	DOTA_PSEUDO_RANDOM_OD_ESSENCE = 12,
	DOTA_PSEUDO_RANDOM_DROW_MARKSMANSHIP = 13,
	DOTA_PSEUDO_RANDOM_OGRE_MAGI_FIREBLAST = 14,
	DOTA_PSEUDO_RANDOM_OGRE_ITEM_MULTICAST = 15,
	DOTA_PSEUDO_RANDOM_SPIRITBREAKER_GREATERBASH = 16,
	DOTA_PSEUDO_RANDOM_LONE_DRUID_ENTANGLE = 17,
	DOTA_PSEUDO_RANDOM_FACELESS_BASH = 18,
	DOTA_PSEUDO_RANDOM_FACELESS_EVADE_SPELL = 19,
	DOTA_PSEUDO_RANDOM_FACELESS_EVADE_ATTACK = 20,
	DOTA_PSEUDO_RANDOM_FACELESS_VOID_BACKTRACK = 21,
	DOTA_PSEUDO_RANDOM_BREWMASTER_CRIT = 22,
	DOTA_PSEUDO_RANDOM_BREWMASTER_CINDER_BREW = 23,
	DOTA_PSEUDO_RANDOM_SNIPER_HEADSHOT = 24,
	DOTA_PSEUDO_RANDOM_ATOS = 25,
	DOTA_PSEUDO_RANDOM_JUGG_CRIT = 26,
	DOTA_PSEUDO_RANDOM_DAZZLE_SCEPTER = 27,
	DOTA_PSEUDO_RANDOM_CHAOS_CRIT = 28,
	DOTA_PSEUDO_RANDOM_LYCAN_CRIT = 29,
	DOTA_PSEUDO_RANDOM_TUSK_CRIT = 30,
	DOTA_PSEUDO_RANDOM_CM_FREEZING_FIELD = 31,
	DOTA_PSEUDO_RANDOM_GENERIC_BASHER = 32,
	DOTA_PSEUDO_RANDOM_SKELETONKING_CRIT = 33,
	DOTA_PSEUDO_RANDOM_SKELETONKING_CRIT_MORTAL = 34,
	DOTA_PSEUDO_RANDOM_ITEM_GREATERCRIT = 35,
	DOTA_PSEUDO_RANDOM_ITEM_LESSERCRIT = 36,
	DOTA_PSEUDO_RANDOM_ITEM_BASHER = 37,
	DOTA_PSEUDO_RANDOM_ITEM_SOLAR_CREST = 38,
	DOTA_PSEUDO_RANDOM_ITEM_JAVELIN_ACCURACY = 39,
	DOTA_PSEUDO_RANDOM_ITEM_TRIDENT = 40,
	DOTA_PSEUDO_RANDOM_ITEM_ABYSSAL = 41,
	DOTA_PSEUDO_RANDOM_ITEM_ABYSSAL_BLOCK = 42,
	DOTA_PSEUDO_RANDOM_ITEM_STOUT = 43,
	DOTA_PSEUDO_RANDOM_ITEM_VANGUARD = 44,
	DOTA_PSEUDO_RANDOM_ITEM_CRIMSON_GUARD = 45,
	DOTA_PSEUDO_RANDOM_ITEM_PMS = 46,
	DOTA_PSEUDO_RANDOM_ITEM_HALBRED_MAIM = 47,
	DOTA_PSEUDO_RANDOM_ITEM_SANGEYASHA_MAIM = 48,
	DOTA_PSEUDO_RANDOM_ITEM_SANGEKAYA_MAIM = 49,
	DOTA_PSEUDO_RANDOM_ITEM_SANGE_MAIM = 50,
	DOTA_PSEUDO_RANDOM_ITEM_BUTTERFLY = 51,
	DOTA_PSEUDO_RANDOM_ITEM_MAELSTROM = 52,
	DOTA_PSEUDO_RANDOM_ITEM_MJOLLNIR = 53,
	DOTA_PSEUDO_RANDOM_ITEM_MJOLLNIR_STATIC = 54,
	DOTA_PSEUDO_RANDOM_ITEM_MKB = 55,
	DOTA_PSEUDO_RANDOM_ITEM_SILVER_EDGE = 56,
	DOTA_PSEUDO_RANDOM_ITEM_NAGINATA = 57,
	DOTA_PSEUDO_RANDOM_TROLL_BASH = 58,
	DOTA_PSEUDO_RANDOM_RIKI_SMOKE_SCREEN = 59,
	DOTA_PSEUDO_RANDOM_CHAOS_DOUBLE_CRIT = 60,
	DOTA_PSEUDO_RANDOM_CHAOS_TRIPLE_CRIT = 61,
	DOTA_PSEUDO_RANDOM_GENERIC_EVASION = 62,
	DOTA_PSEUDO_RANDOM_GENERIC_HEIGHT_MISS = 63,
	DOTA_PSEUDO_RANDOM_GENERIC_MISS = 64,
	DOTA_PSEUDO_RANDOM_ARMADILLO_HEARTPIERCER = 65,
	DOTA_PSEUDO_RANDOM_MARS_SHIELD = 66,
	DOTA_PSEUDO_RANDOM_ANTIMAGE_SILENT = 67,
	DOTA_PSEUDO_RANDOM_NEUTRAL_DROP_TIER1 = 68,
	DOTA_PSEUDO_RANDOM_NEUTRAL_DROP_TIER2 = 69,
	DOTA_PSEUDO_RANDOM_NEUTRAL_DROP_TIER3 = 70,
	DOTA_PSEUDO_RANDOM_NEUTRAL_DROP_TIER4 = 71,
	DOTA_PSEUDO_RANDOM_NEUTRAL_DROP_TIER5 = 72,
	DOTA_PSEUDO_RANDOM_MARS_BULWARK = 73,
	DOTA_PSEUDO_RANDOM_MUERTA_GUNSLINGER = 74,
	DOTA_PSEUDO_RANDOM_TROLL_FERVOR_SHARD = 75,
	DOTA_PSEUDO_RANDOM_CUSTOM_GENERIC = 76,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_1 = 77,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_2 = 78,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_3 = 79,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_4 = 80,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_5 = 81,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_6 = 82,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_7 = 83,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_8 = 84,
	DOTA_PSEUDO_RANDOM_CUSTOM_GAME_9 = 85,
};

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
__int64 __fastcall GenRandomNumber(int* a1)
{
	int v1; // er8
	int v2; // er10
	int v3; // er8
	__int64 v4; // rbx
	__int64 v5; // r9
	int v6; // er10
	int v7; // er11
	int v8; // er8
	int v9; // er11
	int v10; // er8
	int v11; // er11
	int v12; // er8
	int v13; // er11
	int v14; // er8
	int v15; // er11
	int v16; // er8
	int v17; // er11
	int v18; // er8
	int v19; // er11
	int v20; // er8
	int v21; // er11
	int v22; // er8
	int v23; // er11
	int v24; // er8
	int v25; // er11
	int v26; // edx
	int v27; // er9
	__int64 v28; // rdx

	v1 = *a1;
	if (*a1 <= 0 || (v2 = a1[1]) == 0)
	{
		v3 = -v1;
		if (v3 < 1)
			v3 = 1;
		v4 = 39i64;
		*a1 = v3;
		v5 = 40i64;
		v6 = 37;
		do
		{
			v7 = 16807 * *a1 - 0x7FFFFFFF * (*a1 / 127773);
			*a1 = v7;
			if (v7 < 0)
			{
				v7 += 0x7FFFFFFF;
				*a1 = v7;
			}
			if (v4 < 32)
			{
				a1[v5 + 1] = v7;
				v7 = *a1;
			}
			v8 = 16807 * v7 - 0x7FFFFFFF * (v7 / 127773);
			*a1 = v8;
			if (v8 < 0)
			{
				v8 += 0x7FFFFFFF;
				*a1 = v8;
			}
			if (v6 + 1 < 32)
			{
				a1[v5] = v8;
				v8 = *a1;
			}
			v9 = 16807 * v8 - 0x7FFFFFFF * (v8 / 127773);
			*a1 = v9;
			if (v9 < 0)
			{
				v9 += 0x7FFFFFFF;
				*a1 = v9;
			}
			if (v6 < 32)
			{
				a1[v5 - 1] = v9;
				v9 = *a1;
			}
			v10 = 16807 * v9 - 0x7FFFFFFF * (v9 / 127773);
			*a1 = v10;
			if (v10 < 0)
			{
				v10 += 0x7FFFFFFF;
				*a1 = v10;
			}
			if (v6 - 1 < 32)
			{
				a1[v5 - 2] = v10;
				v10 = *a1;
			}
			v11 = 16807 * v10 - 0x7FFFFFFF * (v10 / 127773);
			*a1 = v11;
			if (v11 < 0)
			{
				v11 += 0x7FFFFFFF;
				*a1 = v11;
			}
			if (v6 - 2 < 32)
			{
				a1[v5 - 3] = v11;
				v11 = *a1;
			}
			v12 = 16807 * v11 - 0x7FFFFFFF * (v11 / 127773);
			*a1 = v12;
			if (v12 < 0)
			{
				v12 += 0x7FFFFFFF;
				*a1 = v12;
			}
			if (v6 - 3 < 32)
			{
				a1[v5 - 4] = v12;
				v12 = *a1;
			}
			v13 = 16807 * v12 - 0x7FFFFFFF * (v12 / 127773);
			*a1 = v13;
			if (v13 < 0)
			{
				v13 += 0x7FFFFFFF;
				*a1 = v13;
			}
			if (v6 - 4 < 32)
			{
				a1[v5 - 5] = v13;
				v13 = *a1;
			}
			v14 = 16807 * v13 - 0x7FFFFFFF * (v13 / 127773);
			*a1 = v14;
			if (v14 < 0)
			{
				v14 += 0x7FFFFFFF;
				*a1 = v14;
			}
			if (v6 - 5 < 32)
			{
				a1[v5 - 6] = v14;
				v14 = *a1;
			}
			v15 = 16807 * v14 - 0x7FFFFFFF * (v14 / 127773);
			*a1 = v15;
			if (v15 < 0)
			{
				v15 += 0x7FFFFFFF;
				*a1 = v15;
			}
			if (v6 - 6 < 32)
			{
				a1[v5 - 7] = v15;
				v15 = *a1;
			}
			v16 = 16807 * v15 - 0x7FFFFFFF * (v15 / 127773);
			*a1 = v16;
			if (v16 < 0)
			{
				v16 += 0x7FFFFFFF;
				*a1 = v16;
			}
			if (v6 - 7 < 32)
			{
				a1[v5 - 8] = v16;
				v16 = *a1;
			}
			v17 = 16807 * v16 - 0x7FFFFFFF * (v16 / 127773);
			*a1 = v17;
			if (v17 < 0)
			{
				v17 += 0x7FFFFFFF;
				*a1 = v17;
			}
			if (v6 - 8 < 32)
			{
				a1[v5 - 9] = v17;
				v17 = *a1;
			}
			v18 = 16807 * v17 - 0x7FFFFFFF * (v17 / 127773);
			*a1 = v18;
			if (v18 < 0)
			{
				v18 += 0x7FFFFFFF;
				*a1 = v18;
			}
			if (v6 - 9 < 32)
			{
				a1[v5 - 10] = v18;
				v18 = *a1;
			}
			v19 = 16807 * v18 - 0x7FFFFFFF * (v18 / 127773);
			*a1 = v19;
			if (v19 < 0)
			{
				v19 += 0x7FFFFFFF;
				*a1 = v19;
			}
			if (v6 - 10 < 32)
			{
				a1[v5 - 11] = v19;
				v19 = *a1;
			}
			v20 = 16807 * v19 - 0x7FFFFFFF * (v19 / 127773);
			*a1 = v20;
			if (v20 < 0)
			{
				v20 += 0x7FFFFFFF;
				*a1 = v20;
			}
			if (v6 - 11 < 32)
			{
				a1[v5 - 12] = v20;
				v20 = *a1;
			}
			v21 = 16807 * v20 - 0x7FFFFFFF * (v20 / 127773);
			*a1 = v21;
			if (v21 < 0)
			{
				v21 += 0x7FFFFFFF;
				*a1 = v21;
			}
			if (v6 - 12 < 32)
			{
				a1[v5 - 13] = v21;
				v21 = *a1;
			}
			v22 = 16807 * v21 - 0x7FFFFFFF * (v21 / 127773);
			*a1 = v22;
			if (v22 < 0)
			{
				v22 += 0x7FFFFFFF;
				*a1 = v22;
			}
			if (v6 - 13 < 32)
			{
				a1[v5 - 14] = v22;
				v22 = *a1;
			}
			v23 = 16807 * v22 - 0x7FFFFFFF * (v22 / 127773);
			*a1 = v23;
			if (v23 < 0)
			{
				v23 += 0x7FFFFFFF;
				*a1 = v23;
			}
			if (v6 - 14 < 32)
			{
				a1[v5 - 15] = v23;
				v23 = *a1;
			}
			v24 = 16807 * v23 - 0x7FFFFFFF * (v23 / 127773);
			*a1 = v24;
			if (v24 < 0)
			{
				v24 += 0x7FFFFFFF;
				*a1 = v24;
			}
			if (v6 - 15 < 32)
			{
				a1[v5 - 16] = v24;
				v24 = *a1;
			}
			v25 = 16807 * v24 - 0x7FFFFFFF * (v24 / 127773);
			*a1 = v25;
			if (v25 < 0)
			{
				v25 += 0x7FFFFFFF;
				*a1 = v25;
			}
			if (v6 - 16 < 32)
			{
				a1[v5 - 17] = v25;
				v25 = *a1;
			}
			v1 = 16807 * v25 - 0x7FFFFFFF * (v25 / 127773);
			*a1 = v1;
			v26 = v1;
			if (v1 < 0)
			{
				v1 += 0x7FFFFFFF;
				*a1 = v1;
				v26 = v1;
			}
			if (v6 - 17 < 32)
			{
				a1[v5 - 18] = v26;
				v1 = *a1;
			}
			v6 -= 20;
			v4 -= 20i64;
			v5 -= 20i64;
		} while (v5 >= 1);
		v2 = a1[2];
		a1[1] = v2;
	}
	v27 = 16807 * v1 - 0x7FFFFFFF * (v1 / 127773);
	*a1 = v27;
	if (v27 < 0)
	{
		v27 += 0x7FFFFFFF;
		*a1 = v27;
	}
	v28 = v2 / 0x4000000;
	a1[1] = a1[v28 + 2];
	a1[v28 + 2] = v27;
	return (unsigned int)a1[1];
}

struct CDOTAInput : NormalClass {
	GETTER(int, GetSequenceNumber, 0x3D24);
};

void* oCreateMove;
void hkCreateMove(CDOTAInput* rcx, int slot, bool should_fill_weaponselect) {
	OFUNC(CreateMove)(rcx, slot, should_fill_weaponselect);

	auto v120 = *(DWORD*)((uintptr_t)rcx + 0x3E98 * slot + 0x3F64);
	//auto user_cmd = (CDota2UserCmdPB*)(0x3E98 * slot + 104i64 * ((int)v120 % 150) + (uintptr_t)rcx + 0x250);
	auto user_cmd = (CDota2UserCmdPB*)((char*)rcx + 0x250 + 0x68 * (v120 % 150));
	static auto RandomFloat = Memory::GetExport<Address>("tier0.dll", "RandomFloat");
	static float* rndData = RandomFloat.Offset(0x72).GetAbsoluteAddress(3);

	auto seed = user_cmd->base().random_seed();

	seed = seed;
}

void* oOnDemoHero;
bool hkOnDemoHero(CDOTAGameRules* thisptr, int entIdx, CDOTAClientMsg_DemoHero* msg) {
	//msg->mutable_item_defs()->Set(2, 6996);
	return OFUNC(OnDemoHero)(thisptr, entIdx, msg);
}

void* oRollPseudoRandomPercentage;
bool hkRollPseudoRandomPercentage(int chance, PseudoRandom mode, CBaseEntity* ent) {
	if (mode == DOTA_PSEUDO_RANDOM_PHANTOMASSASSIN_CRIT)
		return 1;

	return OFUNC(RollPseudoRandomPercentage)(chance, mode, ent);
}

void HackThread(HMODULE hModule) {
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	//Modules::SkinChanger.DeleteSOCacheFiles();

	d2c.Initialize(hModule);

	//{
	//	auto Decode = Memory::Scan("E8 ? ? ? ? 44 0F B6 BD ? ? ? ? 44 22 F8 44 88 BD ? ? ? ? E9", "networksystem.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(Decode);
	//}

	//{
	//	auto OnDemoHero = Memory::Scan("48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 8B 05",
	//		"client.dll");
	//	HOOKFUNC(OnDemoHero);
	//}

	{
		auto CreateMove = Memory::Scan("44 88 44 24 ?? 89 54 24 ?? 55 53 56 57 41 54",
			"client.dll");
		HOOKFUNC(CreateMove);
	}

	//{
	//	auto RollPseudoRandomPercentage = Memory::Scan("E8 ? ? ? ? 0F B6 D0 8B 8B","server.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(RollPseudoRandomPercentage);
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