#include "Signatures.h"
#include "../Base/Logging.h"

#define SET_VAR(var, data) var = (data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void Signatures::FindSignatures() {
	CMsg = reinterpret_cast<decltype(CMsg)>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
	CMsgColor = reinterpret_cast<decltype(CMsgColor)>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));

	Log(LP_INFO, "SIGNATURES:");

	//xref: "<BAD GAMEUI STATE>"
	//you can see messages about UI state in console when switching between game/menu/loading screen
	//SCAN_FUNC(CGameUI__ChangeGameUIState, SigScan::Find("E8 ? ? ? ? 48 8B 5C 24 ? C6 86", L"client.dll").GetAbsoluteAddress(1, 5));

	SET_VAR(GetPlayer, SigScan::Find("33 C0 83 F9 FF", L"client.dll"));
	{
		int* iScreenSize = SigScan::Find("89 05 ? ? ? ? 8B CA", L"engine2.dll").GetAbsoluteAddress(2);
		GameData.ScreenSize =
			Vector2D{
				static_cast<float>(iScreenSize[0]),
				static_cast<float>(iScreenSize[1])
		};
	}
	// 2nd vfunc of CDOTA_PanoramaMinimapRenderer
	SET_VAR(CDOTAMinimapRenderer_Render, SigScan::Find("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 56 41 57 48 83 EC 40 F3 0F 10 84 24", L"client.dll"));

	// JS
	SET_VAR(CDOTABaseNPC::GetAttackSpeed, SigScan::Find("E8 ? ? ? ? F3 0F 59 05 ? ? ? ? 0F 57 C9 ", L"client.dll").GetAbsoluteAddress(1));
	// JS
	SET_VAR(IsHUDFlipped, SigScan::Find("48 83 EC 38 B9 ? ? ? ? E8 ? ? ? ? 48 8B C8", L"client.dll"));

	// xref "particles/ui_mouseactions/waypoint_flag.vpcf"
	// The one with the bigger offset from function
	SET_VAR(PrepareUnitOrders, SigScan::Find("E8 ? ? ? ? 49 8B CF FF C5", L"client.dll").GetAbsoluteAddress(1, 5));

	// Xref DestroyParticleEffect to a lea rcx just behind the string
	// It's offset by 9 bytes because it checks for an invalid handle before doing the initial mov
	SET_VAR(CDOTAParticleManager::DestroyParticleFunc, SigScan::Find("E8 ? ? ? ? 48 83 C3 1C", L"client.dll").GetAbsoluteAddress(1, 5));

	// JS function
	SET_VAR(CDOTAGameRules::GetGameTimeFunc, SigScan::Find("E8 ? ? ? ? 8B 04 2E", L"client.dll").GetAbsoluteAddress(1, 5));

	// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
	SET_VAR(WorldToScreen, SigScan::Find("E8 ? ? ? ? 83 7D B7 00", L"client.dll").GetAbsoluteAddress(1, 5));

	// xref: "BuildCacheSubscribed(CEconItem)"
	SET_VAR(CreateEconItem, SigScan::Find("48 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 ? 48 8D 0D", L"client.dll"));
	SET_VAR(CEconItem::DeserializeFromProtobufItemFunc, SigScan::Find("E8 ? ? ? ? 41 0F B6 46 ? 41 84 C4", L"client.dll").GetAbsoluteAddress(1));
	SET_VAR(CEconItem::EnsureCustomDataExistsFunc, SigScan::Find("E8 ? ? ? ? 4C 8B 7E 48", L"client.dll").GetAbsoluteAddress(1));
	// Last call in JS func GetLevelSpecialValueFor for a method of CDOTABaseAbility
	SET_VAR(CDOTABaseAbility::GetLevelSpecialValueForFunc, SigScan::Find("E8 ? ? ? ? F3 0F 5A C0", L"client.dll").GetAbsoluteAddress(1, 5));
	// Second call inside that ^
	SET_VAR(CDOTABaseAbility::GetKVEntry, Address(CDOTABaseAbility::GetLevelSpecialValueForFunc).Offset(0x33).GetAbsoluteAddress(1, 5));

	//xref: "You are #%d in line of %d waiting players.\n"
	SET_VAR(DispatchPacket, SigScan::Find("E8 ? ? ? ? 48 8B CF E8 ? ? ? ? B8", L"client.dll").GetAbsoluteAddress(1, 5));

	//xref: "CProtoBufMsg::BAsyncSendProto"
	SET_VAR(BAsyncSendProto, SigScan::Find("E8 ? ? ? ? 48 8B 4D 90 48 89 7C 24", L"client.dll").GetAbsoluteAddress(1, 5));
	SET_VAR(GetItemSchema, SigScan::Find("E8 ? ? ? ? 48 89 5D 18", L"client.dll").GetAbsoluteAddress(1, 5));
	SET_VAR(CDOTAItemSchema::GetItemDefByIndex, SigScan::Find("E8 ? ? ? ? 83 FE 05", L"client.dll").GetAbsoluteAddress(1, 5));
	SET_VAR(CDOTAItemSchema::GetItemDefArrIdx, Address(CDOTAItemSchema::GetItemDefByIndex).Offset(0x16).GetAbsoluteAddress(1, 5));

	//xref: "RP: Setting %s's status to %s (%s).\n"
	SET_VAR(CDOTARichPresence::SetRPStatusFunc, SigScan::Find("4C 89 4C 24 20 55 53 57 41 54", L"client.dll"));

	//xref: "Failed to get custom game %llu timestamp/CRC!\n"
	SET_VAR(CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby,
		SigScan::Find("E8 ? ? ? ? 48 8B D7 48 8B CB E8 ? ? ? ? 48 8D 8B", L"client.dll")
		.GetAbsoluteAddress(1)
		.Offset(0x27E)
		.GetAbsoluteAddress(1));

	//xref: "cache_%u_%u.soc", closest to the beginning
	SET_VAR(SaveSerializedSOCache, SigScan::Find("4C 8B DC 57 48 81 EC ? ? ? ? 48 8B 05", L"client.dll"));

	//xref: "#DOTA_GlobalItems_Emoticons"
	//decompile in IDA
	//in the middle, 240 lines in is:
	// if(!sub_XXXXXXX(v34, v89)) goto LABEL_XX:
	SET_VAR(BIsEmoticonUnlocked, SigScan::Find("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 45 08", L"client.dll").GetAbsoluteAddress(1, 5));

	//xref "OnColorChanged", lea rax, [XXXXXXXXX] below it
	SET_VAR(CBaseEntity::OnColorChanged, SigScan::Find("40 53 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 8B 01 0F B6 93", L"client.dll"));

	// LoadUITexture = SigScan::Find("57 48 83 EC 20 48 8B 1A 49 8B F0 48 8B FA 48 85 DB", L"panorama.dll").Offset(-10);
}
