#pragma once
#include "../Base/CUtlVector.h"
#include "../Enums.h"
#include "CDOTABaseNPCHero.h"
#include "../Globals/Signatures.h"
#include "../Protobufs/dota_commonmessages.pb.h"

class CDOTAPlayerController : public CBaseEntity {
public:
	uint32_t GetAssignedHeroHandle() {
		return Member< uint32_t>(Netvars::C_DOTAPlayerController::m_hAssignedHero);
	}
	auto GetSelectedUnits() {
		return Member<CUtlVector<uint32_t>>(Netvars::C_DOTAPlayerController::m_nSelectedUnits).AsStdVector();
	}
	CDOTABaseNPC_Hero* GetAssignedHero() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(GetAssignedHeroHandle()));
	}
	uint64_t GetSteamID() {
		return Member<uint64_t>(Netvars::CBasePlayerController::m_steamID);
	}
	void CastNoTarget(ENT_HANDLE handle, CBaseEntity* issuer = nullptr) {
		if (issuer == nullptr)
			issuer = GetAssignedHero();
		PrepareOrder(DOTA_UNIT_ORDER_CAST_NO_TARGET,
			0,
			&Vector::Zero,
			H2IDX(handle),
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			issuer,
			false,
			true
		);
	}
	void BuyItem(int itemId) {
		PrepareOrder(DOTA_UNIT_ORDER_PURCHASE_ITEM,
			1,
			&Vector::Zero,
			itemId,
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			GetAssignedHero(),
			false,
			true
		);
	}
	void OrderMoveTo(Vector* pos, bool directMovement = false, CBaseEntity* issuer = nullptr) {
		if (issuer == nullptr)
			issuer = GetAssignedHero();

		PrepareOrder(directMovement ? DOTA_UNIT_ORDER_MOVE_TO_DIRECTION : DOTA_UNIT_ORDER_MOVE_TO_POSITION,
			0,
			pos,
			0,
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			issuer,
			false,
			true
		);
	}
	void PrepareOrder(dotaunitorder_t orderType, uint32_t targetIndex, Vector* position, uint32_t abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue = false, bool showEffects = true) {
		//if (issuer == nullptr)
		//	issuer = GetAssignedHero();

		if (Signatures::PrepareUnitOrders == nullptr)
			return;

		Signatures::PrepareUnitOrders(this, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CDOTAPlayerController>("CDOTAPlayerController", sol::base_classes, sol::bases<CBaseEntity>());
		type["GetAssignedHeroHandle"] = &CDOTAPlayerController::GetAssignedHeroHandle;
		type["GetSelectedUnits"] = &CDOTAPlayerController::GetSelectedUnits;
		type["GetAssignedHero"] = &CDOTAPlayerController::GetAssignedHero;
		type["GetSteamID"] = &CDOTAPlayerController::GetSteamID;

		type["PrepareOrder"] = &CDOTAPlayerController::PrepareOrder;
		type["CastNoTarget"] = &CDOTAPlayerController::CastNoTarget;
		type["OrderMoveTo"] = &CDOTAPlayerController::OrderMoveTo;
		type["BuyItem"] = &CDOTAPlayerController::BuyItem;
	}
};
