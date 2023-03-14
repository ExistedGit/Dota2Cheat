#pragma once
#include "../Base/CUtlVector.h"
#include "../Enums.h"
#include "CDOTABaseNPCHero.h"
#include "../Globals/Signatures.h"
#include "../Protobufs/dota_commonmessages.pb.h"

class CDOTAPlayerController : public CBaseEntity {
public:
	GETTER(uint32_t, GetAssignedHeroHandle, Netvars::C_DOTAPlayerController::m_hAssignedHero);
	GETTER(uint64_t, GetSteamID, Netvars::CBasePlayerController::m_steamID);

	std::vector<uint32_t> GetSelectedUnits();
	CDOTABaseNPC_Hero* GetAssignedHero();
	
	void CastNoTarget(ENT_HANDLE handle, CBaseEntity* issuer = nullptr);
	void BuyItem(int itemId);
	void OrderMoveTo(Vector* pos, bool directMovement = false, CBaseEntity* issuer = nullptr);
	void PrepareOrder(dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue = false, bool showEffects = true);

	static void BindLua(sol::state& lua);
};
