#include "Signatures.h"

#define SET_VAR(var, data) var = (data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void Signatures::FindSignatures() {
	CMsg = Memory::GetExport("tier0.dll", "Msg");
	CMsgColor = Memory::GetExport("tier0.dll", "?ConColorMsg@@YAXAEBVColor@@PEBDZZ");

	SignatureDB::ParseSignatures(NamedSignatures);

	// Log(LP_INFO, "SIGNATURES:");

	//xref: "<BAD GAMEUI STATE>"
	//you can see messages about UI state in console when switching between game/menu/loading screen
	//SCAN_FUNC(CGameUI__ChangeGameUIState, Memory::Scan("E8 ? ? ? ? 48 8B 5C 24 ? C6 86", "client.dll").GetAbsoluteAddress(1, 5));

	//SET_VAR(GetPlayer, Memory::Scan("33 C0 83 F9 FF", "client.dll"));
	////{
	////	int* iScreenSize = Memory::Scan("89 05 ? ? ? ? 8B CA", L"engine2.dll").GetAbsoluteAddress(2);
	////	GameData.ScreenSize =
	////		Vector2D{
	//// 			static_cast<float>(iScreenSize[0]),
	////			static_cast<float>(iScreenSize[1])
	////	};
	////}
	
	//// 2nd vfunc of CDOTA_PanoramaMinimapRenderer
	//SET_VAR(CDOTAPanoramaMinimapRenderer__Render, Memory::Scan("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 56 41 57 48 83 EC 40 F3 0F 10 84 24", "client.dll"));

	//// JS
	//SET_VAR(CDOTABaseNPC::GetAttackSpeed, Memory::Scan("E8 ? ? ? ? F3 0F 59 05 ? ? ? ? 0F 57 C9", "client.dll").GetAbsoluteAddress(1));

	//// xref "particles/ui_mouseactions/waypoint_flag.vpcf"
	//// The one with the bigger offset from function
	//SET_VAR(PrepareUnitOrders, Memory::Scan("E8 ? ? ? ? F3 44 0F 11 5B", "client.dll").GetAbsoluteAddress(1, 5));

	//// Xref DestroyParticleEffect to a lea rcx just behind the string
	//SET_VAR(CDOTAParticleManager::DestroyParticleFunc, Memory::Scan("E8 ? ? ? ? 48 83 C3 1C", "client.dll").GetAbsoluteAddress(1, 5));

	//// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
	//SET_VAR(WorldToScreen, Memory::Scan("E8 ? ? ? ? 39 6C 24 68", "client.dll").GetAbsoluteAddress(1, 5));

	//// xref: "BuildCacheSubscribed(CEconItem)"
	//SET_VAR(CreateEconItem, Memory::Scan("48 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 ? 48 8D 0D", "client.dll"));
	//SET_VAR(CEconItem::DeserializeFromProtobufItemFunc, Memory::Scan("E8 ? ? ? ? 41 0F B6 46 ? A8 01", "client.dll").GetAbsoluteAddress(1));
	//SET_VAR(CEconItem::EnsureCustomDataExistsFunc, Memory::Scan("E8 ? ? ? ? 4C 8B 7E 48", "client.dll").GetAbsoluteAddress(1));

	//// JS func
	//SET_VAR(CDOTABaseAbility::GetLevelSpecialValueForFunc, Memory::Scan("E8 ? ? ? ? F3 0F 5A C0", "client.dll").GetAbsoluteAddress(1, 5));

	////xref: "You are #%d in line of %d waiting players.\n"
	//SET_VAR(DispatchPacket, Memory::Scan("E8 ? ? ? ? 48 8B CF E8 ? ? ? ? B8", "client.dll").GetAbsoluteAddress(1));

	////xref: "CProtoBufMsg::BAsyncSendProto"
	//SET_VAR(BAsyncSendProto, Memory::Scan("E8 ? ? ? ? 48 8B 4D 90 48 89 7C 24", "client.dll").GetAbsoluteAddress(1));
	//SET_VAR(GetItemSchema, Memory::Scan("E8 ? ? ? ? 48 89 5D 18", "client.dll").GetAbsoluteAddress(1));
		
	////xref: "RP: Setting %s's status to %s (%s).\n"
	//SET_VAR(CDOTARichPresence::SetRPStatusFunc, Memory::Scan("4C 89 4C 24 20 55 53 57 41 54", "client.dll"));

	////xref: "Failed to get custom game %llu timestamp/CRC!\n"
	//SET_VAR(CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby,
	//	Memory::Scan("E8 ? ? ? ? 48 8B D7 48 8B CB E8 ? ? ? ? 48 8D 8B", "client.dll")
	//	.GetAbsoluteAddress(1)
	//	.Offset(0x27E)
	//	.GetAbsoluteAddress(1));

	////xref: "cache_%u_%u.soc", closest to the beginning
	//SET_VAR(SaveSerializedSOCache, Memory::Scan("4C 8B DC 57 48 81 EC ? ? ? ? 48 8B 05", "client.dll"));

	////xref: "#DOTA_GlobalItems_Emoticons"
	////decompile in IDA
	////in the middle, 240 lines in is:
	//// if(!sub_XXXXXXX(v34, v89)) goto LABEL_XX:
	//SET_VAR(BIsEmoticonUnlocked, Memory::Scan("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 45 08", "client.dll").GetAbsoluteAddress(1, 5));

#if defined(_DEBUG) && !defined(_TESTING)
	SET_VAR(CDOTAItemSchema::GetItemDefByIndex, Memory::Scan("E8 ? ? ? ? 83 FE 05", "client.dll").GetAbsoluteAddress(1));
#endif
}
