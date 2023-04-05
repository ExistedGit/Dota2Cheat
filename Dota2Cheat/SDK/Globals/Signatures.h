#pragma once
#include "../../include.h"
#include "../Base/Memory.h"

#include "Context.h"

#include "../Enums.h"
#include "../Protobufs/dota_commonmessages.pb.h"
#include "../GameSystems/CDOTAParticleManager.h"
#include "../GameSystems/C_DOTAGameRules.h"

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

	typedef void(__fastcall* BIsEmoticonUnlockedFn)(void* thisptr, uint32_t unk);
	inline BIsEmoticonUnlockedFn BIsEmoticonUnlocked{};

	typedef void(__fastcall* CMsgFn)(const char* format, ...);
	typedef void(__fastcall* ColorMsgFn)(Color* color, const char* format, ...);

	typedef void(__fastcall* PrepareUnitOrdersFn)(CDOTAPlayerController* player, dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);
	
	typedef void(__fastcall* WorldToScreenFn)(Vector* coord, int* outX, int* outY, void* offset);

	typedef CDOTAPlayerController* (*GetPlayerFn)(int);
	inline GetPlayerFn GetPlayer{};

	typedef char(*OnAcceptMatchFn)(VClass**);
	inline OnAcceptMatchFn OnAcceptMatch{};

	typedef bool (*LoadUITextureFn)(void* thisptr, void** texturePtr, const char* textureName);

	typedef bool (*BAsyncSendProtoFn)(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID);
	typedef bool (*DispatchPacketFn)(void*, IMsgNetPacket*);

	typedef void(*OnAddModifierFn)(CDOTAModifier*, int);
	typedef void(*OnRemoveModifierFn)(CDOTAModifier*);
	//there's actually no point in sigging it
	//netmessages ftw
	//typedef CDOTAParticleManager::CNewParticleEffect* (__fastcall* CreateLinearProjectile)(void* thisptr, void* creationInfo);

	inline LoadUITextureFn LoadUITexture{};

	inline CMsgFn CMsg{};
	inline ColorMsgFn CMsgColor{};

	using CParticleCollection = void;
	typedef CParticleCollection* (*CreateParticleCollectionFn)(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex);
	
	inline PrepareUnitOrdersFn PrepareUnitOrders{};
	
	inline DispatchPacketFn DispatchPacket{};
	inline BAsyncSendProtoFn BAsyncSendProto{};

	inline CreateParticleCollectionFn CreateParticleCollection{};

	inline WorldToScreenFn WorldToScreen{};
	void FindSignatures(bool log);
}
