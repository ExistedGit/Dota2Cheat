#include "Signatures.h"

#ifdef _DEBUG
#define SCAN_FUNC(var, scan) var = (scan); std::cout << #var << ": " << var << '\n';
#else
#define SCAN_FUNC(var, scan) var = scan;
#endif

void Signatures::FindSignatures(bool log) {
	CMsg = reinterpret_cast<CMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
	CMsgColor = reinterpret_cast<ColorMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));

	SigScanContext ssctx(ctx.CurProcHandle, ctx.CurProcId);
	if (log)
		std::cout << "[SIGNATURES]\n";


	// xref "particles/ui_mouseactions/waypoint_flag.vpcf"
	SCAN_FUNC(PrepareUnitOrders, ssctx.Scan("4C 89 4C 24 20 44 89 44 24 18 89 54 24 10 55 53 56 41 55 41 56 48 8D 6C 24 C0", L"client.dll"));

	// xref "%c [%2.2f] Removing buff '%s' index: %d to '%s'.\n"
	// the sig is to a call instruction
	SCAN_FUNC(OnRemoveModifier, ssctx.Scan("E8 ? ? ? ? FF CB 48 83 EF 01", L"client.dll").GetAbsoluteAddress(1, 5));

	// Xref DestroyParticleEffect to a lea rcx just behind the string
	// It's offset by 9 bytes because it checks for an invalid handle before doing the initial mov
	SCAN_FUNC(CDOTAParticleManager::DestroyParticleFunc, ssctx.Scan("48 89 6C 24 18 56 48 83 EC 30 48 63 81 80 00 00 00 41 0F B6 E8 48 89 5C 24 40 48 8B F1", L"client.dll").Offset(-9));
	SCAN_FUNC(CreateParticleCollection, ssctx.Scan("41 56 48 83 EC 40 4C 89 41 50 48 8B F1 49 8B 01", L"client.dll").Offset(-5));
	SCAN_FUNC(CDOTAGameRules::GetGameTimeFunc, ssctx.Scan("48 89 5C 24 18 48 89 6C 24 20 57 48 83 EC 20 48 8B ? ? ? ? ? 8B DA", L"client.dll"));

	// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
	SCAN_FUNC(WorldToScreen, ssctx.Scan("56 57 41 56 48 83 EC 60 49 8B F0 4C 8B F2 48 8B F9 4D 85 C9", L"client.dll").Offset(-5));

	// Last call in JS func GetLevelSpecialValueFor for a method of CDOTABaseAbility
	SCAN_FUNC(CDOTABaseAbility::GetLevelSpecialValueForFunc, ssctx.Scan("48 89 5C 24 08 57 48 83 EC 30 48 8B ? ? ? ? ? 41 8B D9", L"client.dll").Offset(0x9B).GetAbsoluteAddress(1, 5));
	// Second call inside that ^
	SCAN_FUNC(CDOTABaseAbility::GetKVEntry, Address(CDOTABaseAbility::GetLevelSpecialValueForFunc).Offset(0x33).GetAbsoluteAddress(1, 5));

	//xref: "You are #%d in line of %d waiting players.\n"
	SCAN_FUNC(DispatchPacket, ssctx.Scan("74 05 48 8B 01 FF 10 48 8B 06 48 8B CE 48 89 BC 24 80 00 00 00", L"client.dll").Offset(-0x1B));

	//xref "CProtoBufMsg::BAsyncSendProto"
	SCAN_FUNC(BAsyncSendProto, ssctx.Scan("40 53 41 54 48 83 EC 58 48 83 79 10 00 4C 8B E2 48 8B D9 75 0A", L"client.dll"));

	//xref "OnColorChanged", lea rax, [XXXXXXXXX] below it
	SCAN_FUNC(CBaseEntity::OnColorChanged, ssctx.Scan("40 53 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 8B 01 0F B6 93", L"client.dll"));
	// LoadUITexture = ssctx.Scan("57 48 83 EC 20 48 8B 1A 49 8B F0 48 8B FA 48 85 DB", L"panorama.dll").Offset(-10);
}
