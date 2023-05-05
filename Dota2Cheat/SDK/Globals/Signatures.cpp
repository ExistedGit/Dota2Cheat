#include "Signatures.h"
#include "../Base/Logging.h"
#include <curl/curl.h>

#define SET_VAR(var, data) var = (data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	{ LogF(LP_ERROR, "{}: {}", #var, (void*)var); error = true; }

size_t Signatures::WriteRemoteString(void* ptr, size_t size, size_t nmemb, void* stream) {
	std::string data((const char*)ptr, (size_t)size * nmemb);
	*((std::stringstream*)stream) << data;
	return size * nmemb;
}

void Signatures::ParseSignatures(nlohmann::json data) {
	for (auto& [sigName, sigVar] : NamedSignatures) {
		if (!data.contains(sigName))
			continue;

		auto& info = data[sigName];
		std::string sigStr = info["signature"], sigModule = info["module"];
		auto result = SigScan::Find(sigStr, sigModule);

		if (!result)
			continue;

		if (info.contains("steps")) {
			for (auto& pair : info["steps"].items()) {
				int type = pair.value()[0], value = pair.value()[1];
				switch (type) {
				case 0:
					result = result.GetAbsoluteAddress(value);
					break;
				case 1:
					result = result.Offset(value);
					break;
				}
			}
		}
		*sigVar = result;
	}

	bool brokenSig = false;
	for (auto& [sigName, sigVar] : NamedSignatures) {

		LogF(*sigVar ? LP_DATA : LP_ERROR, "{}: {}", sigName, *sigVar);
		if (!(*sigVar))
			brokenSig = true;
	}
	if (brokenSig)
		system("pause");
	
}

void Signatures::LoadSignaturesFromNetwork(const std::string& url) {
	std::cout << std::format("Loading signatures from {}\n", url);

	std::stringstream out;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteRemoteString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dota2Cheat/0.1");
	CURLcode CURLresult = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (CURLresult != CURLE_OK) {
		std::cout << "FAILED TO LOAD SIGNATURES FROM " << url << '\n';
		system("pause");
	}

	ParseSignatures(nlohmann::json::parse(out.str()));
}

void Signatures::FindSignatures() {
	bool error = false;

	CMsg = reinterpret_cast<decltype(CMsg)>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
	CMsgColor = reinterpret_cast<decltype(CMsgColor)>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));

	// Log(LP_INFO, "SIGNATURES:");

	//xref: "<BAD GAMEUI STATE>"
	//you can see messages about UI state in console when switching between game/menu/loading screen
	//SCAN_FUNC(CGameUI__ChangeGameUIState, SigScan::Find("E8 ? ? ? ? 48 8B 5C 24 ? C6 86", "client.dll").GetAbsoluteAddress(1, 5));

	//SET_VAR(GetPlayer, SigScan::Find("33 C0 83 F9 FF", "client.dll"));
	////{
	////	int* iScreenSize = SigScan::Find("89 05 ? ? ? ? 8B CA", L"engine2.dll").GetAbsoluteAddress(2);
	////	GameData.ScreenSize =
	////		Vector2D{
	//// 			static_cast<float>(iScreenSize[0]),
	////			static_cast<float>(iScreenSize[1])
	////	};
	////}
	//// 2nd vfunc of CDOTA_PanoramaMinimapRenderer
	//SET_VAR(CDOTAPanoramaMinimapRenderer__Render, SigScan::Find("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 56 41 57 48 83 EC 40 F3 0F 10 84 24", "client.dll"));

	//// JS
	//SET_VAR(CDOTABaseNPC::GetAttackSpeed, SigScan::Find("E8 ? ? ? ? F3 0F 59 05 ? ? ? ? 0F 57 C9", "client.dll").GetAbsoluteAddress(1));
	//// JS
	//SET_VAR(IsHUDFlipped, SigScan::Find("48 83 EC 38 B9 ? ? ? ? E8 ? ? ? ? 48 8B C8", "client.dll"));

	//// xref "particles/ui_mouseactions/waypoint_flag.vpcf"
	//// The one with the bigger offset from function
	//SET_VAR(PrepareUnitOrders, SigScan::Find("E8 ? ? ? ? F3 44 0F 11 5B", "client.dll").GetAbsoluteAddress(1, 5));

	//// Xref DestroyParticleEffect to a lea rcx just behind the string
	//// It's offset by 9 bytes because it checks for an invalid handle before doing the initial mov
	//SET_VAR(CDOTAParticleManager::DestroyParticleFunc, SigScan::Find("E8 ? ? ? ? 48 83 C3 1C", "client.dll").GetAbsoluteAddress(1, 5));

	//// JS function
	//SET_VAR(CDOTAGameRules::GetGameTimeFunc, SigScan::Find("E8 ? ? ? ? 8B 04 2E", "client.dll").GetAbsoluteAddress(1, 5));

	//// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
	//SET_VAR(WorldToScreen, SigScan::Find("E8 ? ? ? ? 39 6C 24 68", "client.dll").GetAbsoluteAddress(1, 5));

	//// xref: "BuildCacheSubscribed(CEconItem)"
	//SET_VAR(CreateEconItem, SigScan::Find("48 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 ? 48 8D 0D", "client.dll"));
	//SET_VAR(CEconItem::DeserializeFromProtobufItemFunc, SigScan::Find("E8 ? ? ? ? 41 0F B6 46 ? A8 01", "client.dll").GetAbsoluteAddress(1));
	//SET_VAR(CEconItem::EnsureCustomDataExistsFunc, SigScan::Find("E8 ? ? ? ? 4C 8B 7E 48", "client.dll").GetAbsoluteAddress(1));

	//// JS func
	//SET_VAR(CDOTABaseAbility::GetLevelSpecialValueForFunc, SigScan::Find("E8 ? ? ? ? F3 0F 5A C0", "client.dll").GetAbsoluteAddress(1, 5));

	////xref: "You are #%d in line of %d waiting players.\n"
	//SET_VAR(DispatchPacket, SigScan::Find("E8 ? ? ? ? 48 8B CF E8 ? ? ? ? B8", "client.dll").GetAbsoluteAddress(1));

	////xref: "CProtoBufMsg::BAsyncSendProto"
	//SET_VAR(BAsyncSendProto, SigScan::Find("E8 ? ? ? ? 48 8B 4D 90 48 89 7C 24", "client.dll").GetAbsoluteAddress(1));
	//SET_VAR(GetItemSchema, SigScan::Find("E8 ? ? ? ? 48 89 5D 18", "client.dll").GetAbsoluteAddress(1));

	////xref: "RP: Setting %s's status to %s (%s).\n"
	//SET_VAR(CDOTARichPresence::SetRPStatusFunc, SigScan::Find("4C 89 4C 24 20 55 53 57 41 54", "client.dll"));

	////xref: "Failed to get custom game %llu timestamp/CRC!\n"
	//SET_VAR(CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby,
	//	SigScan::Find("E8 ? ? ? ? 48 8B D7 48 8B CB E8 ? ? ? ? 48 8D 8B", "client.dll")
	//	.GetAbsoluteAddress(1)
	//	.Offset(0x27E)
	//	.GetAbsoluteAddress(1));

	////xref: "cache_%u_%u.soc", closest to the beginning
	//SET_VAR(SaveSerializedSOCache, SigScan::Find("4C 8B DC 57 48 81 EC ? ? ? ? 48 8B 05", "client.dll"));

	////xref: "#DOTA_GlobalItems_Emoticons"
	////decompile in IDA
	////in the middle, 240 lines in is:
	//// if(!sub_XXXXXXX(v34, v89)) goto LABEL_XX:
	//SET_VAR(BIsEmoticonUnlocked, SigScan::Find("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 45 08", "client.dll").GetAbsoluteAddress(1, 5));

	////xref "OnColorChanged", lea rax, [XXXXXXXXX] below it
	//SET_VAR(CBaseEntity::OnColorChanged, SigScan::Find("40 53 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 8B 01 0F B6 93", "client.dll"));

	SET_VAR(CDOTAItemSchema::GetItemDefByIndex, SigScan::Find("E8 ? ? ? ? 8B 4E 64", "client.dll").GetAbsoluteAddress(1));
	SET_VAR(CDOTAItemSchema::GetItemDefArrIdx, Address(CDOTAItemSchema::GetItemDefByIndex).Offset(0x16).GetAbsoluteAddress(1));
}
