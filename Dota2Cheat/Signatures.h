#pragma once
#include "patternscan.h"
#include "Enums.h"
#include "sdk.h"
#include "Wrappers.h"
#include <map>
#include "SDK/color.h"
#include "MinHook.h"

namespace Signatures {
	typedef void(__fastcall* CMsgFn)(const char* format, ...);
	typedef void(__fastcall* ColorMsgFn)(Color* color, const char* format, ...);

	typedef void(__fastcall* PrepareUnitOrdersFn)(DotaPlayer* player, DotaUnitOrder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue, bool showEffects);
	typedef void(__fastcall* EntityCallback)(void* thisptr);
	typedef void(__fastcall* WorldToScreenFn)(Vector3* coord, int* outX, int* outY, void* offset);
	typedef void(__fastcall* DestroyParticleFn)(void* thisptr, ENT_HANDLE handle, bool unk);
	typedef double(__fastcall* GetLevelSpecialValueForFn)(void* thisptr, int abilityIndex, const char* value, int level);


	typedef bool (*BAsyncSendProtoFn)(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID);
	typedef bool (*DispatchPacketFn)(void*, IMsgNetPacket*);

	inline CMsgFn CMsg = nullptr;
	inline ColorMsgFn CMsgColor = nullptr;

	inline PrepareUnitOrdersFn PrepareUnitOrders = nullptr;
	inline DispatchPacketFn DispatchPacket = nullptr;
	inline BAsyncSendProtoFn BAsyncSendProto = nullptr;

	inline EntityCallback OnColorChanged = nullptr;
	inline DestroyParticleFn DestroyParticle = nullptr;


	namespace Scripts {
		inline WorldToScreenFn WorldToScreen = nullptr;
		inline GetLevelSpecialValueForFn GetLevelSpecialValueFor = nullptr;
		//		inline EntGetter GetCastRange = nullptr;
	}

	inline void InitSignatures() {
		CMsg = reinterpret_cast<CMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
		CMsgColor = reinterpret_cast<ColorMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));

		char funcAddr[256];
		char funcAddrMask[256];

		//prepareUnitOrders
		ParseCombo("4C 89 4C 24 20 44 89 44 24 18 89 54 24 10 55 53 57 41 55 41 57 48 8D 6C 24 C0", funcAddr, funcAddrMask);
		PrepareUnitOrders = (PrepareUnitOrdersFn)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask);


		// Xref DestroyParticleEffect to a lea rcx just behind the string
		// It's offset by 9 bytes because it checks for an invalid handle before doing the initial mov
		ParseCombo("48 89 6C 24 18 56 48 83 EC 30 48 63 81 80 00 00 00 41 0F B6 E8 48 89 5C 24 40 48 8B F1", funcAddr, funcAddrMask);
		DestroyParticle = (DestroyParticleFn)
			((uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask)
				- 9);

		// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
		ParseCombo("56 57 41 56 48 83 EC 60 49 8B F0 4C 8B F2 48 8B F9 4D 85 C9", funcAddr, funcAddrMask);
		Scripts::WorldToScreen = (WorldToScreenFn)(
			(uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask)
			- 5 // Skipping function prologue to shorten the sig
			);

		ParseCombo("57 48 83 EC 30 48 8B ? ? ? ? ? 41 8B D9 49 8B F8 48 85 C0 74 34", funcAddr, funcAddrMask);
		Scripts::GetLevelSpecialValueFor = (GetLevelSpecialValueForFn)(
			(uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask)
			- 5 // Skipping function prologue to shorten the sig
			);

		//xref: "You are #%d in line of %d waiting players.\n"
		ParseCombo("74 05 48 8B 01 FF 10 48 8B 06 48 8B CE 48 89 BC 24 80 00 00 00", funcAddr, funcAddrMask);
		DispatchPacket = (DispatchPacketFn)(
			(uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask)
			- 0x1B
			);

		//xref "CProtoBufMsg::BAsyncSendProto"
		ParseCombo("40 53 41 54 48 83 EC 58 48 83 79 10 00 4C 8B E2 48 8B D9 75 0A", funcAddr, funcAddrMask);
		BAsyncSendProto = (BAsyncSendProtoFn)(PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask));

		const char* _funcAddr = "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x89\x00\x00\x00\x00\x48\x8B\x01\x0F\xB6\x93";
		const char* _funcAddrMask = "xxxxxxxxxxxx????xxxxxx";
		OnColorChanged = (EntityCallback)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", _funcAddr, _funcAddrMask);

	}

	inline void LogSignatures() {
		std::cout << "[SIGNATURES]\n";
		std::cout << "PrepareUnitOrders: " << PrepareUnitOrders << std::endl;
		std::cout << "OnColorChanged: " << OnColorChanged << std::endl;
		std::cout << "WorldToScreen: " << Scripts::WorldToScreen << std::endl;
		std::cout << "DestroyParticle: " << DestroyParticle << std::endl;
		std::cout << "DispatchPacket: " << DispatchPacket << std::endl;
		std::cout << "BAsyncSendProto: " << BAsyncSendProto << std::endl;
	}
}
