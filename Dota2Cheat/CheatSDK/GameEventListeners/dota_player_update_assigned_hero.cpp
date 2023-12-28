#include "dota_player_update_assigned_hero.h"

void dota_player_update_assigned_hero_l::FireGameEvent(CGameEvent* ev) {
	if (!ctx.localPlayer)
		return;

	int playerid = ev->GetInt("playerid", -2);
	if (playerid != ctx.localPlayer->GetPlayerID())
		return;

	MatchStateManager.OnUpdatedAssignedHero();

	if (false) {
		using namespace Netvars;

		auto hero = ctx.localHero;


		static auto ei = Modules::SkinChanger.CreateItem(6996);

		CEconWearable* w = hero->Wearables()[2];

		w->CallVFunc<12>(0);

		auto item = w->GetAttributeManager()->GetItem();

		item->Field<bool>(C_EconItemView::m_bInitialized) = false;
		w->Field<bool>(C_DOTAWearableItem::m_bHiddenByCombiner) = false;
		w->Field<bool>(C_DOTAWearableItem::m_bIsGeneratingEconItem) = true;

		//static Function init = Memory::Scan("E8 ? ? ? ? 8B C6 3B F3", "client.dll").GetAbsoluteAddress(1);
		static Function OnDataChanged = Memory::Scan("E8 ? ? ? ? EB 42 E8", "client.dll").GetAbsoluteAddress(1);

		//init(item, ei, 0xff);
		//item->Field<bool>(C_EconItemView::m_bInitialized) = false;

		item->Field<uint64_t>(C_EconItemView::m_iItemID) = ei->m_ulID;
		item->Field<uint32_t>(C_EconItemView::m_iItemDefinitionIndex) = ei->m_unDefIndex;

		//auto renderComp = w->Member<VClass*>(C_BaseEntity::m_pRenderComponent);
		//auto renderComp2 = w->Member<VClass*>(C_BaseModelEntity::m_CRenderComponent);
		//renderComp->Field<bool>(0xb0) = false;
		//renderComp2->Field<bool>(0xb0) = false;

		//w->CallVFunc<6>(0);
		//w->CallVFunc<97>(0);

		hero->Field<bool>(0x10cd) = true;
		hero->Field<bool>(0x1739) = true;
		
		w->CallVFunc<6>(0);
		OnDataChanged(hero, 0);
	}
}
