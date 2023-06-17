#pragma once
#include "../Base/Memory.h"
#include "SignatureDB.h"

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
	typedef void* (*CDOTA_DB_Popup_AcceptMatchFn)(VClass*, Panorama::CPanel2D*, const char*, Panorama::CPanel2D*, bool, const char*);
	inline CDOTA_DB_Popup_AcceptMatchFn CDOTA_DB_Popup_AcceptMatch{};

	inline CDOTAItemSchema* (*GetItemSchema)() = nullptr;
	inline CEconItem* (*CreateEconItem)() = nullptr;

	// typedef void* (*CGameUI__ChangeGameUIStateFn)(void* thisptr, DOTAGameUIState_t state);
	// inline CGameUI__ChangeGameUIStateFn CGameUI__ChangeGameUIState{};

	// I don't know what its actual name is, but it's called every frame and has a call with xref "Minimap Objects"
	// which in client.dylib corresponds to CDotaMinimapRenderer's Render function
	// typedef void* (*CDOTAPanoramaMinimapRenderer__RenderFn)(void* thisptr, void*, void**, void*, float, float, float, float);
	// inline CDOTAPanoramaMinimapRenderer__RenderFn CDOTAPanoramaMinimapRenderer__Render{};

	typedef void* (__fastcall* SaveSerializedSOCacheFn)(void* thisptr);
	inline SaveSerializedSOCacheFn SaveSerializedSOCache{};

	inline void(__fastcall* CMsg)(const char* format, ...);
	inline void(__fastcall* CMsgColor)(Color* color, const char* format, ...);

	typedef void(__fastcall* PrepareUnitOrdersFn)(CDOTAPlayerController* player, dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);

	//typedef bool (*LoadUITextureFn)(void* thisptr, void** texturePtr, const char* textureName);

	typedef bool (*BAsyncSendProtoFn)(CProtobufMsgBase<>* protobufMsg, IProtoBufSendHandler* handler);
	typedef bool (*DispatchPacketFn)(void*, IMsgNetPacket*);

	typedef void(*OnAddModifierFn)(CDOTAModifier*, int);
	typedef void(*OnRemoveModifierFn)(CDOTAModifier*);

	//inline LoadUITextureFn LoadUITexture{};

	using CParticleCollection = void;
	//typedef CParticleCollection* (*CreateParticleCollectionFn)(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex);
	//CreateParticleCollectionFn CreateParticleCollection{};

	inline void* (__fastcall* PlayUISoundScript)(void** unk, const char* sound, int unk2);

	inline void(__fastcall* WorldToScreen)(const Vector* coord, int* outX, int* outY, void* offset);
	inline PrepareUnitOrdersFn PrepareUnitOrders{};

	inline DispatchPacketFn DispatchPacket{};
	inline BAsyncSendProtoFn BAsyncSendProto{};

	// state:
	// true = accept
	// false = decline
	inline bool(*CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby)(void* thisptr, bool state);

	static inline std::map<std::string, void**> NamedSignatures{
		SIGMAP_ENTRY(WorldToScreen),
		SIGMAP_ENTRY(PrepareUnitOrders),

#if defined(_DEBUG) && !defined(_TESTING)
		SIGMAP_ENTRY(DispatchPacket),
		SIGMAP_ENTRY(BAsyncSendProto),
		SIGMAP_ENTRY(GetItemSchema),
		SIGMAP_ENTRY(CreateEconItem),
		SIGMAP_ENTRY(SaveSerializedSOCache),
#endif

		SIGMAP_ENTRY(PlayUISoundScript),
		SIGMAP_ENTRY(CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby),
		SIGMAP_ENTRY(CDOTA_DB_Popup_AcceptMatch),

		SIGMAP_ENTRY(CDOTABaseNPC::GetAttackSpeed),

		{"CDOTAParticleManager::DestroyParticle", (void**)&CDOTAParticleManager::DestroyParticleFunc},
		{"CDOTARichPresence::SetRPStatus", (void**)&CDOTARichPresence::SetRPStatusFunc},
		{"CDOTABaseAbility::GetLevelSpecialValueFor", (void**)&CDOTABaseAbility::GetLevelSpecialValueForFunc},
		{"CEconItem::DeserializeFromProtobufItem", (void**)&CEconItem::DeserializeFromProtobufItemFunc},
		{"CEconItem::EnsureCustomDataExists", (void**)&CEconItem::EnsureCustomDataExistsFunc},
	};

	void FindSignatures();
}
