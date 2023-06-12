#pragma once
#include "../Base/CUtlVector.h"
#include "../Enums.h"
#include "CDOTABaseNPCHero.h"
#include "../Globals/Signatures.h"
#include "../Protobufs/dota_commonmessages.pb.h"

struct Order {
	dotaunitorder_t Type{};
	uint32_t
		TargetIndex{},
		AbilityIndex{};
	Vector Position{};
	bool
		Queue = false,
		ShowEffects = true;
	PlayerOrderIssuer_t IssuerType = DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY;
	CBaseEntity* Issuer{};

#define BUILDER_SETTER(var) \
	Order& Set##var(decltype(var) var){ \
		this->var = var; return *this; \
	}

	BUILDER_SETTER(Type);
	BUILDER_SETTER(TargetIndex);
	BUILDER_SETTER(AbilityIndex);
	BUILDER_SETTER(Position);
	BUILDER_SETTER(Queue);
	BUILDER_SETTER(ShowEffects);
	BUILDER_SETTER(Issuer);
	BUILDER_SETTER(IssuerType);

#undef BUILDER_SETTER
};

class CDOTAPlayerController : public CBaseEntity {
public:
	GETTER(CHandle<CDOTABaseNPC_Hero>, GetAssignedHeroHandle, Netvars::C_DOTAPlayerController::m_hAssignedHero);
	GETTER(int32_t, GetSequenceNum, Netvars::C_DOTAPlayerController::m_nOutgoingOrderSequenceNumber);
	GETTER(uint32_t, GetPlayerID, Netvars::C_DOTAPlayerController::m_nPlayerID);
	GETTER(uint64_t, GetSteamID, Netvars::CBasePlayerController::m_steamID);

	std::vector<uint32_t> GetSelectedUnits();
	CDOTABaseNPC_Hero* GetAssignedHero();


	void CastNoTarget(CDOTABaseAbility* ability, CBaseEntity* issuer = nullptr);
	void CastTarget(CDOTABaseAbility* ability, CBaseEntity* target, CBaseEntity* issuer = nullptr);
	void BuyItem(int itemId);
	void OrderMoveTo(Vector* pos, bool directMovement = false, CBaseEntity* issuer = nullptr);
	void PrepareOrder(dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue = false, bool showEffects = true);
	void PrepareOrder(const Order& order) {
		PrepareOrder(order.Type, order.TargetIndex, (Vector*)&order.Position, order.AbilityIndex, order.IssuerType, order.Issuer, order.Queue, order.ShowEffects);
	}
	static void BindLua(sol::state& lua);
};
