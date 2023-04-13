#pragma once
#include "../../include.h"
#include "../Base/Memory.h"

#include "Context.h"

#include "../Enums.h"
#include "../Protobufs/dota_commonmessages.pb.h"
#include "../GameSystems/CDOTAParticleManager.h"
#include "../GameSystems/C_DOTAGameRules.h"
#include "../GameSystems/CDOTARichPresence.h"

#include "../Interfaces/GC/CGCClient.h"
#include "../Interfaces/GC/ItemSchema.h"
#include "../Interfaces/GC/CEconItem.h"

#include "../Interfaces/Network/CNetworkMessages.h"
#include "../Entities/CDOTABaseAbility.h"

class CDOTAPlayerController;
class CDOTAModifier;

namespace Signatures {
	inline CDOTAItemSchema* (*GetItemSchema)() = nullptr;

	inline CEconItem* (*CreateEconItem)() = nullptr;

	typedef void* (*CGameUI__ChangeGameUIStateFn)(void* thisptr, DOTAGameUIState_t state);
	inline CGameUI__ChangeGameUIStateFn CGameUI__ChangeGameUIState;

	typedef bool(__fastcall* BIsEmoticonUnlockedFn)(void* thisptr, uint32_t unk);
	inline BIsEmoticonUnlockedFn BIsEmoticonUnlocked{};

	typedef void* (__fastcall* SaveSerializedSOCacheFn)(void* thisptr);
	inline SaveSerializedSOCacheFn SaveSerializedSOCache{};

	inline void(__fastcall* CMsg)(const char* format, ...);
	inline void(__fastcall* CMsgColor)(Color* color, const char* format, ...);

	typedef void(__fastcall* PrepareUnitOrdersFn)(CDOTAPlayerController* player, dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);
	
	inline CDOTAPlayerController* (*GetPlayer)(int idx);

	//typedef bool (*LoadUITextureFn)(void* thisptr, void** texturePtr, const char* textureName);

	typedef bool (*BAsyncSendProtoFn)(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID);
	typedef bool (*DispatchPacketFn)(void*, IMsgNetPacket*);

	typedef void(*OnAddModifierFn)(CDOTAModifier*, int);
	typedef void(*OnRemoveModifierFn)(CDOTAModifier*);

	//inline LoadUITextureFn LoadUITexture{};

	using CParticleCollection = void;
	//typedef CParticleCollection* (*CreateParticleCollectionFn)(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex);
	//CreateParticleCollectionFn CreateParticleCollection{};
	
	inline void(__fastcall* WorldToScreen)(Vector* coord, int* outX, int* outY, void* offset);
	inline PrepareUnitOrdersFn PrepareUnitOrders{};
	
	inline DispatchPacketFn DispatchPacket{};
	inline BAsyncSendProtoFn BAsyncSendProto{};

	// state:
	// true = accept
	// false = decline
	inline bool(*CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby)(void* thisptr, bool state);

	void FindSignatures();
}
