#pragma once
#include "../Base/PatternScan.h"

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
#include <json.hpp>
#include <regex>

class CDOTAPlayerController;
class CDOTAModifier;

namespace Signatures {
	inline CDOTAItemSchema* (*GetItemSchema)() = nullptr;
	inline bool(*IsHUDFlipped)() = nullptr;
	inline CEconItem* (*CreateEconItem)() = nullptr;

	typedef void* (*CGameUI__ChangeGameUIStateFn)(void* thisptr, DOTAGameUIState_t state);
	inline CGameUI__ChangeGameUIStateFn CGameUI__ChangeGameUIState{};

	// I don't know what its actual name is, but it's called every frame and has a call with xref "Minimap Objects"
	// which in client.dylib corresponds to CDotaMinimapRenderer's Render function
	typedef void* (*CDOTAPanoramaMinimapRenderer__RenderFn)(void* thisptr, void*, void**, void*, float, float, float, float);
	inline CDOTAPanoramaMinimapRenderer__RenderFn CDOTAPanoramaMinimapRenderer__Render{};

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

	inline void* (__fastcall *PlayUISoundScript)(void** unk, const char* sound, int unk2);

	inline void(__fastcall* WorldToScreen)(const Vector* coord, int* outX, int* outY, void* offset);
	inline PrepareUnitOrdersFn PrepareUnitOrders{};

	inline DispatchPacketFn DispatchPacket{};
	inline BAsyncSendProtoFn BAsyncSendProto{};

	// state:
	// true = accept
	// false = decline
	inline bool(*CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby)(void* thisptr, bool state);


#define SIG_NAMED(var) {#var, (void**)&var}
	static inline std::map<std::string, void**> NamedSignatures{
		SIG_NAMED(WorldToScreen),
		SIG_NAMED(DispatchPacket),
		SIG_NAMED(BAsyncSendProto),
		SIG_NAMED(PrepareUnitOrders),
		SIG_NAMED(IsHUDFlipped),
		SIG_NAMED(GetItemSchema),
		SIG_NAMED(CreateEconItem),
		SIG_NAMED(GetPlayer),
		SIG_NAMED(SaveSerializedSOCache),
		SIG_NAMED(BIsEmoticonUnlocked),
		SIG_NAMED(PlayUISoundScript),
		SIG_NAMED(CDOTAGCClientSystem__SendReadyUpMessageForCurrentLobby),

		SIG_NAMED(CBaseEntity::OnColorChanged),
		SIG_NAMED(CDOTABaseNPC::GetAttackSpeed),

		{"CDOTAParticleManager::DestroyParticle", (void**)&CDOTAParticleManager::DestroyParticleFunc},
		{"CDOTAGameRules::GetGameTime", (void**)&CDOTAGameRules::GetGameTimeFunc},
		{"CDOTARichPresence::SetRPStatus", (void**)&CDOTARichPresence::SetRPStatusFunc},
		{"CDOTABaseAbility::GetLevelSpecialValueFor", (void**)&CDOTABaseAbility::GetLevelSpecialValueForFunc},
	};
	size_t WriteRemoteString(void* ptr, size_t size, size_t nmemb, void* stream);
	void ParseSignatures(nlohmann::json data);
	void LoadSignaturesFromNetwork(const std::string& url);
	
	void FindSignatures();
}
