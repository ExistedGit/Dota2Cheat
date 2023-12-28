#pragma once
#include "SignatureDB.h"

#include "../../SDK/Base/Logging.h"
#include "../../SDK/Enums.h"
#include "../../SDK/Protobufs/dota_commonmessages.pb.h"

#include "../../SDK/GameSystems/CDOTAParticleManager.h"
#include "../../SDK/GameSystems/C_DOTAGameRules.h"
#include "../../SDK/GameSystems/CDOTARichPresence.h"

#include "../../SDK/Interfaces/GC/CGCClient.h"
#include "../../SDK/Interfaces/GC/ItemSchema.h"
#include "../../SDK/Interfaces/GC/CEconItem.h"

#include "../../SDK/Interfaces/Network/CNetworkMessages.h"

#include "../../SDK/Entities/CDOTABaseAbility.h"
#include "../../SDK/Entities/CDOTABaseNPC.h"


#include "../../SDK/Panorama/CUIEngineSource2.h"

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

	void FindSignatures();
}
