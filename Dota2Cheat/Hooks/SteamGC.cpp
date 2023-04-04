#include "SteamGC.h"

bool Hooks::HookSteamGC() {
	auto GetSteamPipe = GetExport("steam_api64.dll", "GetHSteamPipe");
	auto GetSteamUser = GetExport("steam_api64.dll", "GetHSteamUser");
	auto steam_interface = VMT(
		Interfaces::SteamClient
		->GetISteamGenericInterface(GetSteamUser(), GetSteamPipe(), "SteamGameCoordinator001")
	);

	return HookFunc(steam_interface.GetVM(0), &hkISteamGCSendMessage, &oISteamGCSendMessage, "SteamGameCoordinator::SendMessage")
		&& HookFunc(steam_interface.GetVM(2), &hkISteamGCRetrieveMessage, &oISteamGCRetrieveMessage, "SteamGameCoordinator::RetrieveMessage");
	//HookFunc(steam_interface.GetVM(1), &hook::functions::ISteamGameCoordinator__IsMessageAvailable, reinterpret_cast<LPVOID*>(&hook::original::fpSClientIsMessageAvailable), "SteamGameCoordinator::IsMessageAvailable");
}
