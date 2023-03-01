#pragma once
#include "patternscan.h"
#include "Enums.h"
#include "sdk.h"
#include <map>
#include "SDK/color.h"

class DotaPlayer;
class DotaModifier;

namespace Signatures {
	typedef void(__fastcall* CMsgFn)(const char* format, ...);
	typedef void(__fastcall* ColorMsgFn)(Color* color, const char* format, ...);

	typedef void(__fastcall* PrepareUnitOrdersFn)(DotaPlayer* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue, bool showEffects);
	typedef void(__fastcall* EntityCallback)(void* thisptr);
	typedef void(__fastcall* WorldToScreenFn)(Vector3* coord, int* outX, int* outY, void* offset);
	typedef void(__fastcall* DestroyParticleFn)(void* thisptr, ENT_HANDLE handle, bool unk);
	typedef double(__fastcall* GetLevelSpecialValueForFn)(void* thisptr, int abilityIndex, const char* value, int level);

	//there's actually no point in sigging it
	//netmessages ftw
	//typedef CDOTAParticleManager::CNewParticleEffect* (__fastcall* CreateLinearProjectile)(void* thisptr, void* creationInfo);

	typedef bool (*BAsyncSendProtoFn)(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID);
	typedef bool (*DispatchPacketFn)(void*, IMsgNetPacket*);

	inline CMsgFn CMsg = nullptr;
	inline ColorMsgFn CMsgColor = nullptr;

	using CParticleCollection = void;
	typedef CParticleCollection* (*CreateParticleCollectionFn)(void* particleSystemMgr, void*, void*, void*, bool, float, int);

	typedef void(*OnAddModifierFn)(DotaModifier*, int);
	typedef void(*OnRemoveModifierFn)(DotaModifier*, void*, void*);

	inline PrepareUnitOrdersFn PrepareUnitOrders = nullptr;
	inline DispatchPacketFn DispatchPacket = nullptr;
	inline BAsyncSendProtoFn BAsyncSendProto = nullptr;

	inline EntityCallback OnColorChanged = nullptr;
	inline DestroyParticleFn DestroyParticle = nullptr;

	inline CreateParticleCollectionFn CreateParticleCollection = nullptr;
	inline OnRemoveModifierFn OnRemoveModifier = nullptr;

	namespace Scripts {
		inline WorldToScreenFn WorldToScreen = nullptr;
		inline GetLevelSpecialValueForFn GetLevelSpecialValueFor = nullptr;
	}

	// Default signature scan
#define SIGSCAN(func, sig, dll) ParseCombo(sig, funcAddr, funcAddrMask); \
	func =(decltype(func))PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, dll, funcAddr, funcAddrMask); \
	if(log) std::cout << #func << ": " << func << '\n'

	// Signature scan with offset from the found address, I have plenty of such signatures
#define SIGSCAN_OFF(func, sig, dll, offset) ParseCombo(sig, funcAddr, funcAddrMask); \
	func = (decltype(func))((uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, dll, funcAddr, funcAddrMask) + (offset)); \
	if(log) std::cout << #func << ": " << func << '\n'

	inline void InitSignatures(bool log) {
		CMsg = reinterpret_cast<CMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
		CMsgColor = reinterpret_cast<ColorMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));

		if (log)
			std::cout << "[SIGNATURES]\n";

		char funcAddr[256];
		char funcAddrMask[256];


		SIGSCAN(PrepareUnitOrders, "4C 89 4C 24 20 44 89 44 24 18 89 54 24 10 55 53 57 41 55 41 57 48 8D 6C 24 C0", L"client.dll");

		ParseCombo("E8 ? ? ? ? 48 83 ED 01 79 DF", funcAddr, funcAddrMask);
		OnRemoveModifier = (OnRemoveModifierFn)(
			GetAbsoluteAddress(
				(uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask),
				1, 5
			)
			);

		// Xref DestroyParticleEffect to a lea rcx just behind the string
		// It's offset by 9 bytes because it checks for an invalid handle before doing the initial mov
		SIGSCAN_OFF(DestroyParticle, "48 89 6C 24 18 56 48 83 EC 30 48 63 81 80 00 00 00 41 0F B6 E8 48 89 5C 24 40 48 8B F1", L"client.dll", -9);
		SIGSCAN_OFF(CreateParticleCollection, "41 56 48 83 EC 40 4C 89 41 50 48 8B F1 49 8B 01", L"client.dll", -5);

		// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
		SIGSCAN_OFF(Scripts::WorldToScreen, "56 57 41 56 48 83 EC 60 49 8B F0 4C 8B F2 48 8B F9 4D 85 C9", L"client.dll", -5);
		SIGSCAN_OFF(Scripts::GetLevelSpecialValueFor, "57 48 83 EC 30 48 8B ? ? ? ? ? 41 8B D9 49 8B F8 48 85 C0 74 34", L"client.dll", -5);

		//xref: "You are #%d in line of %d waiting players.\n"
		SIGSCAN_OFF(DispatchPacket, "74 05 48 8B 01 FF 10 48 8B 06 48 8B CE 48 89 BC 24 80 00 00 00", L"client.dll", -0x1B);

		//xref "CProtoBufMsg::BAsyncSendProto"
		SIGSCAN(BAsyncSendProto, "40 53 41 54 48 83 EC 58 48 83 79 10 00 4C 8B E2 48 8B D9 75 0A", L"client.dll");

		//xref "OnColorChanged", lea rax, [XXXXXXXXX] below it
		SIGSCAN(OnColorChanged, "40 53 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 8B 01 0F B6 93", L"client.dll");
	}
}
