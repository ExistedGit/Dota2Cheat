#pragma once
#include "SignatureDB.h"

#include <Base/Logging.h>
#include <Enums.h>
#include <Protobufs/dota_commonmessages.pb.h>

#include <GameSystems/CDOTAParticleManager.h>
#include <GameSystems/C_DOTAGameRules.h>
#include <GameSystems/CDOTARichPresence.h>

#include <Interfaces/GC/CGCClient.h>
#include <Interfaces/GC/ItemSchema.h>
#include <Interfaces/GC/CEconItem.h>

#include <Interfaces/Network/CNetworkMessages.h>

#include <Entities/CDOTABaseAbility.h>
#include <Entities/CDOTABaseNPC.h>

#include <Panorama/CUIEngineSource2.h>

class CDOTAPlayerController;
class CDOTAModifier;

namespace Signatures {
	inline CDOTAItemSchema* (*GetItemSchema)() = nullptr;
	inline CEconItem* (*CreateEconItem)() = nullptr;

	inline void* SaveSerializedSOCache{};

	inline void(__fastcall* CMsg)(const char* format, ...);
	inline void(__fastcall* CMsgColor)(Color* color, const char* format, ...);

	typedef bool(__fastcall* PrepareUnitOrdersFn)(CDOTAPlayerController* player, dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);


	typedef bool (*BAsyncSendProtoFn)(CProtobufMsgBase<>* protobufMsg, IProtoBufSendHandler* handler);
	typedef bool (*DispatchPacketFn)(void*, IMsgNetPacket*);

	inline CDOTAPlayerController* (*GetPlayer)(int idx);

	inline PrepareUnitOrdersFn PrepareUnitOrders{};

	inline DispatchPacketFn DispatchPacket{};
	inline BAsyncSendProtoFn BAsyncSendProto{};

#define SIGMAP_ENTRY(var) {#var, (void**)&var}
	
	static inline std::map<std::string, void**> NamedSignatures{
		SIGMAP_ENTRY(PrepareUnitOrders),

#if defined(_DEBUG) && !defined(_TESTING)
		//SIGMAP_ENTRY(DispatchPacket),
		//SIGMAP_ENTRY(BAsyncSendProto),
		SIGMAP_ENTRY(GetItemSchema),
		SIGMAP_ENTRY(CreateEconItem),
		SIGMAP_ENTRY(SaveSerializedSOCache),
		{"CEconItem::DeserializeFromProtobufItem", (void**)&CEconItem::DeserializeFromProtobufItemFunc},
		{"CEconItem::EnsureCustomDataExists", (void**)&CEconItem::EnsureCustomDataExistsFunc},
#endif

		SIGMAP_ENTRY(GetPlayer),
		SIGMAP_ENTRY(CDOTABaseNPC::GetAttackSpeed),

		// {"CDOTARichPresence::SetRPStatus", (void**)&CDOTARichPresence::SetRPStatusFunc},
		{"CDOTABaseAbility::GetLevelSpecialValueFor", (void**)&CDOTABaseAbility::GetLevelSpecialValueForFunc},
	};

#undef SIGMAP_ENTRY

	void FindSignatures();
}
