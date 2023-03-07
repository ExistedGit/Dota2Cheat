#pragma once
#include "../../include.h"
#include "../Base/Memory.h"
#include "../SigScan/patternscan.h"

#include "Context.h"

#include "../Enums.h"
#include "../Protobufs/dota_commonmessages.pb.h"
#include "../GameSystems/CDOTAParticleManager.h"
#include "../Interfaces/CGCClient.h"
#include "../Interfaces/Network/CNetworkMessages.h"
#include "../Entities/CDOTABaseAbility.h"

class CDOTAPlayerController;
class CDOTAModifier;

namespace Signatures {
	typedef void(__fastcall* CMsgFn)(const char* format, ...);
	typedef void(__fastcall* ColorMsgFn)(Color* color, const char* format, ...);

	typedef void(__fastcall* PrepareUnitOrdersFn)(CDOTAPlayerController* player, dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);
	typedef void(__fastcall* WorldToScreenFn)(Vector* coord, int* outX, int* outY, void* offset);

	//there's actually no point in sigging it
	//netmessages ftw
	//typedef CDOTAParticleManager::CNewParticleEffect* (__fastcall* CreateLinearProjectile)(void* thisptr, void* creationInfo);

	typedef bool (*LoadUITextureFn)(void* thisptr, void** texturePtr, const char* textureName);
	inline LoadUITextureFn LoadUITexture{};

	typedef bool (*BAsyncSendProtoFn)(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID);
	typedef bool (*DispatchPacketFn)(void*, IMsgNetPacket*);

	inline CMsgFn CMsg = nullptr;
	inline ColorMsgFn CMsgColor = nullptr;

	using CParticleCollection = void;
	typedef CParticleCollection* (*CreateParticleCollectionFn)(void* particleSystemMgr, void*, void*, void*, bool, float, int);

	typedef void(*OnAddModifierFn)(CDOTAModifier*, int);
	typedef void(*OnRemoveModifierFn)(CDOTAModifier*, void*, void*);

	inline PrepareUnitOrdersFn PrepareUnitOrders = nullptr;
	inline DispatchPacketFn DispatchPacket = nullptr;
	inline BAsyncSendProtoFn BAsyncSendProto = nullptr;


	inline CreateParticleCollectionFn CreateParticleCollection = nullptr;
	inline OnRemoveModifierFn OnRemoveModifier = nullptr;

	namespace Scripts {
		inline WorldToScreenFn WorldToScreen = nullptr;
	}

	// Default signature scan
#define SIGSCAN(func, sig, dll) ParseCombo(sig, funcAddr, funcAddrMask); \
	func =(decltype(func))PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, dll, funcAddr, funcAddrMask)

#define SIGSCAN_LOG(func, sig, dll) SIGSCAN(func, sig, dll); \
	if(log) std::cout << #func << ": " << func << '\n'

	// Signature scan with offset from the found address, I have plenty of such signatures
#define SIGSCAN_OFF(func, sig, dll, offset) ParseCombo(sig, funcAddr, funcAddrMask); \
	func = (decltype(func))((uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, dll, funcAddr, funcAddrMask) + (offset)); \
	if(log) std::cout << #func << ": " << func << '\n'

	inline void FindSignatures(bool log) {
		CMsg = reinterpret_cast<CMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
		CMsgColor = reinterpret_cast<ColorMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));

		if (log)
			std::cout << "[SIGNATURES]\n";

		char funcAddr[256];
		char funcAddrMask[256];
		SIGSCAN_LOG(PrepareUnitOrders, "4C 89 4C 24 20 44 89 44 24 18 89 54 24 10 55 53 56 41 55 41 56 48 8D 6C 24 C0", L"client.dll");

		ParseCombo("E8 ? ? ? ? 48 83 ED 01 79 AF", funcAddr, funcAddrMask);
		OnRemoveModifier = (OnRemoveModifierFn)(
			GetAbsoluteAddress(
				(uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask),
				1, 5
			)
			);

		// Xref DestroyParticleEffect to a lea rcx just behind the string
		// It's offset by 9 bytes because it checks for an invalid handle before doing the initial mov
		SIGSCAN_OFF(CDOTAParticleManager::DestroyParticleFunc, "48 89 6C 24 18 56 48 83 EC 30 48 63 81 80 00 00 00 41 0F B6 E8 48 89 5C 24 40 48 8B F1", L"client.dll", -9);
		SIGSCAN_OFF(CreateParticleCollection, "41 56 48 83 EC 40 4C 89 41 50 48 8B F1 49 8B 01", L"client.dll", -5);

		// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
		SIGSCAN_OFF(Scripts::WorldToScreen, "56 57 41 56 48 83 EC 60 49 8B F0 4C 8B F2 48 8B F9 4D 85 C9", L"client.dll", -5);
		SIGSCAN_OFF(CDOTABaseAbility::GetLevelSpecialValueForFunc, "57 48 83 EC 30 48 8B ? ? ? ? ? 41 8B D9 49 8B F8 48 85 C0 74 34", L"client.dll", -5);

		//xref: "You are #%d in line of %d waiting players.\n"
		SIGSCAN_OFF(DispatchPacket, "74 05 48 8B 01 FF 10 48 8B 06 48 8B CE 48 89 BC 24 80 00 00 00", L"client.dll", -0x1B);

		//xref "CProtoBufMsg::BAsyncSendProto"
		SIGSCAN_LOG(BAsyncSendProto, "40 53 41 54 48 83 EC 58 48 83 79 10 00 4C 8B E2 48 8B D9 75 0A", L"client.dll");

		//xref "OnColorChanged", lea rax, [XXXXXXXXX] below it
		SIGSCAN_LOG(CBaseEntity::OnColorChanged, "40 53 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 8B 01 0F B6 93", L"client.dll");
		SIGSCAN_OFF(LoadUITexture, "57 48 83 EC 20 48 8B 1A 49 8B F0 48 8B FA 48 85 DB", L"panorama.dll", -10);
	};
}
