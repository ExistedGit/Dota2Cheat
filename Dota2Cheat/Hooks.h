#pragma once
#include "vtabler.h"
#include "MatchStateHandling.h"
#include "AutoBuyTome.h"
#include "AutoUseMidas.h"
#include "AutoUseMagicWand.h"
#include "Wrappers.h"
#include "UIState.h"
#include "Input.h"
#include "include/google/protobuf/message.h"
#include "SDK/INetChannel.h"
#include "CDOTAParticleManager.h"
#include <future>
#include <algorithm>
#include <optional>

#include "SpiritBreakerChargeHighlighter.h"
#include "SunStrikeHighlighter.h"
#include "VBE.h"
#include "IllusionColoring.h"
#include "AegisAutoPickup.h"

extern bool IsInMatch;
extern std::vector<BaseNpc*> enemyHeroes;
extern CDOTAParticleManager::ParticleWrapper particleWrap;


namespace VMTs {
	inline std::unique_ptr<VMT> UIEngine = nullptr;
	inline std::unique_ptr<VMT> Engine = nullptr;
	inline std::unique_ptr<VMT> Entity = nullptr;
	inline std::unique_ptr<VMT> NetworkSystem = nullptr;
	inline std::unique_ptr<VMT> NetChannel = nullptr;
}

namespace Hooks {
	typedef BaseEntity* (*OnAddEntityFn)(CEntitySystem*, BaseEntity*, ENT_HANDLE);
	typedef void (*RunFrameFn)(u64, u64);

	template<typename T = BaseEntity>
	inline std::vector<T*> GetEntitiesByFilter(const std::vector<const char*>& filters) {
		std::vector<T*> vec{};
		for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
			auto* ent = Interfaces::EntitySystem->GetEntity(i);
			if (ent == nullptr || ent->GetIdentity()->IsDormant())
				continue;
			//std::cout << ent->SchemaBinding() << '\n';
			const char* className = ent->SchemaBinding()->binaryName;
			if (className != nullptr && TestStringFilters(className, filters))
				vec.push_back((T*)ent);
		}
		return vec;
	};



	//inline bool test = false;
	inline void EntityIteration() {
		int illusionCount = 0;
		bool midasUsed = false;
		bool runePickUp = false;

		ENT_HANDLE midas = AutoUseMidasCheck(assignedHero);

		for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
			auto* ent = Interfaces::EntitySystem->GetEntity(i);

			if (ent == nullptr || ent->GetIdentity()->IsDormant())
				continue;
			const char* className = ent->SchemaBinding()->binaryName;

			if (className == nullptr)
				continue;
			if (!midasUsed && midas != -1
				&& strstr(className, "Creep")) {

				auto creep = (BaseNpc*)ent;

				//neutral prefixes because Wildwing Ripper and Dark Troll Warlord spawn a tornado and skeletons respectively
				//they have their summoner's name in them but not the word "neutral"
				static std::vector<const char*> filters = {
						"ranged",
						"flagbearer",
						"siege",
						"alpha_wolf",
						"centaur_khan",
						"neutral_dark_troll_warlord",
						"ursa_warrior",
						"ogre_magi",
						"satyr_hellcaller",
						"neutral_enraged_wildkin"
				};
				auto midasEnt = Interfaces::EntitySystem->GetEntity < BaseAbility>(H2IDX(midas));

				// If the creep is visible, not one of ours, is alive, is within 600 hammer units and its name matches one of the filters
				if (creep->GetTeam() != assignedHero->GetTeam() &&
					creep->GetHealth() > 0 &&
					!creep->IsWaitingToSpawn() &&
					IsWithinRadius(creep->GetPos2D(), assignedHero->GetPos2D(), midasEnt->GetEffectiveCastRange()) &&
					TestStringFilters(creep->GetUnitName(), filters)) {
					midasUsed = true;
					localPlayer->PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TARGET, i, &Vector3::Zero, H2IDX(midas), PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, assignedHero);
				}
			}
			else if (Config::AutoRunePickupEnabled && !runePickUp && strstr(className, "C_DOTA_Item_Rune")) {
				auto* rune = (ItemRune*)ent;
				if (rune->GetRuneType() == DotaRunes::BOUNTY &&
					IsWithinRadius(rune->GetPos2D(), assignedHero->GetPos2D(), 150.0f)
					) {
					localPlayer->PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_PICKUP_RUNE, i, &Vector3::Zero, 0, PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_HERO_ONLY, assignedHero, false, false);
				}
			}
			else {
				Modules::IllusionColoring.ColorIfIllusion(ent);
				Modules::AegisAutoPickup.PickUpIfAegis(ent);
			}

		}
	}

	inline void UpdateCameraDistance() {
		static auto varInfo = CVarSystem::CVar["dota_camera_distance"];
		if (Config::CameraDistance != varInfo.var->value.flt) {
			varInfo.var->value.flt = Config::CameraDistance;
			Interfaces::CVar->TriggerCallback(varInfo);
		}
	}
	inline void UpdateWeather() {
		static auto varInfo = CVarSystem::CVar["cl_weather"];
		if (Config::WeatherListIdx != varInfo.var->value.i32) {
			varInfo.var->value.i32 = Config::WeatherListIdx;
		}
	}

	inline void RunFrame(u64 a, u64 b) {
		static bool isInGame = Interfaces::Engine->IsInGame();

		if (isInGame) {
			//std::cout << "frame\n";
			if (IsInMatch) {

				UpdateCameraDistance();
				UpdateWeather();
				Modules::SunStrikeHighlighter.FrameBasedLogic();

				if (assignedHero->GetLifeState() == 0) { // if alive
					// VBE: m_flStartSequenceCycle updates 30 times a second
					// It doesn't update when you are seen(stays at zero)


					AutoUseWandCheck(assignedHero, Config::AutoHealWandHPTreshold, Config::AutoHealWandMinCharges);
					AutoUseFaerieFireCheck(assignedHero, Config::AutoHealFaerieFireHPTreshold);
					Modules::AutoBuyTome.FrameBasedLogic();
					Modules::VBE.FrameBasedLogic();
					Modules::SBChargeHighlighter.FrameBasedLogic();

					EntityIteration();
				}
#ifdef _DEBUG
				if (IsKeyPressed(VK_NUMPAD8)) {
					auto selected = localPlayer->GetSelectedUnits();
					auto ent = Interfaces::EntitySystem->GetEntity<BaseNpc>(selected[0]);
					auto pos = ent->GetPos();

					std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
						<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
						<< "\n\tAttack Time: " << clamp(ent->GetBaseAttackTime() / ent->GetAttackSpeed(), 0.24, 2)
						<< "\n\t" << "IsRoshan: " << ent->IsRoshan()
						//<< "\n\t" << "GetCastRangeBonus: " << std::dec << Function(0x00007FFAEE5C0B00).Execute<int>(nullptr, ent->GetIdentity()->entHandle)
						<< '\n';
				}
				if (IsKeyPressed(VK_NUMPAD7)) {
					//LogInvAndAbilities(ent);
					//auto ab = ent->GetAbilities()[2];
					//std::cout << std::dec << selected[0] << " " << std::dec<< ab.name << " " << ab.GetAs<BaseAbility>()->GetCooldown() << '\n';
					//LogEntities();
				}
				if (IsKeyPressed(VK_NUMPAD3)) {
					//std::cout << assignedHero->GetForwardVector(10) << '\n';
					//LogEntities();
					auto vec = GetEntitiesByFilter({
						"Tower"
						});
					for (auto& ent : vec) {
						const char* className = ent->SchemaBinding()->binaryName;
						std::cout << className << ' ' << std::dec << H2IDX(ent->GetIdentity()->entHandle)
							<< std::hex << " -> " << ent << '\n';
					}
					//auto midas = assignedHero->FindItemBySubstring("blink");
					//if (HVALID(midas.handle))
					//	std::cout << std::dec << midas.GetAs<BaseAbility>()->GetVFunc(0xf4).ptr << '\n';

				}
				if (IsKeyPressed(VK_HOME)) {
					std::cout << Interfaces::EntitySystem->GetEntity(H2IDX(3637677)) << '\n';
					std::cout << Interfaces::EntitySystem->GetEntity(328) << '\n';
					//auto item = assignedHero->FindItemBySubstring("orb");
					//if (HVALID(item.handle)) {
					//	auto ent = item.GetAs<BaseAbility>();
					//	//auto pos = ent->GetPos();
					//	std::cout << std::dec << Signatures::Scripts::GetLevelSpecialValueFor(nullptr, H2IDX(item.handle), "bonus_all_stats", 1) << '\n';
					//}
				}
#endif
			}
		}
		VMTs::UIEngine->GetOriginalMethod<RunFrameFn>(6)(a, b);
	}

	inline BaseEntity* OnAddEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
		auto className = ent->SchemaBinding()->binaryName;
		if (className != nullptr) {
			if (TestStringFilters(className, { "Item_Physical" })) {
				physicalItems.push_back(ent);
			}
			else if (TestStringFilters(className, { "BaseNPC" })) {
				const char* idName = ent->GetIdentity()->GetName();
				if (Modules::SunStrikeHighlighter.SunStrikeIncoming &&
					idName == nullptr)
					Modules::SunStrikeHighlighter.QueueThinker(ent);

			}
		}

		return VMTs::Entity->GetOriginalMethod<OnAddEntityFn>(14)(thisptr, ent, handle);
	};
	inline BaseEntity* OnRemoveEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
		auto iter = std::find(physicalItems.begin(), physicalItems.end(), ent);
		if (iter != physicalItems.end())
			physicalItems.erase(iter);
		return VMTs::Entity->GetOriginalMethod<OnAddEntityFn>(15)(thisptr, ent, handle);
	}

	// for MinHook
	inline Signatures::PrepareUnitOrdersFn PrepareUnitOrdersOriginal = nullptr;
	inline Signatures::DispatchPacketFn DispatchPacketOriginal = nullptr;
	inline Signatures::BAsyncSendProtoFn BAsyncSendProtoOriginal = nullptr;

	inline std::future<void> manaAbusePickup;
	inline void hkPrepareUnitOrders(DotaPlayer* player, DotaUnitOrder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue, bool showEffects) {
		//std::cout << "[ORDER] " << player << '\n';
		bool giveOrder = true; // whether or not the function will continue
		switch (orderType) {
		case DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TARGET:
		{
			//Redirects spell casts from illusions to the real hero
			auto npc = Interfaces::EntitySystem->GetEntity<BaseNpc>(targetIndex);

			if (strstr(npc->SchemaBinding()->binaryName, "C_DOTA_Unit_Hero") &&
				reinterpret_cast<BaseNpcHero*>(npc)->IsIllusion()) {
				auto assignedHero = Interfaces::EntitySystem->GetEntity<DotaPlayer>(
					H2IDX(
						npc->GetOwnerEntityHandle()
					)
					)
					->GetAssignedHero();
				if (assignedHero->IsTargetable()) {
					targetIndex =
						H2IDX(
							assignedHero
							->GetIdentity()
							->entHandle
						);
					showEffects = false;
				}
			}
			break;
		}
		case DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_POSITION: {
			// Blink overshoot bypass
			auto item = Interfaces::EntitySystem->GetEntity<BaseAbility>(abilityIndex);
			if (strstr(item->GetIdentity()->GetName(), "blink")) {
				auto maxDist = item->GetEffectiveCastRange();
				auto pos2D = *(Vector2*)position;
				if (!IsWithinRadius(issuer->GetPos2D(), pos2D, maxDist)) {
					auto dist = issuer->GetPos2D().DistanceTo(pos2D);
					// Relative vector from the hero to the click point
					auto vec = Vector2(pos2D.x - issuer->GetPos2D().x, pos2D.y - issuer->GetPos2D().y);
					// -1% to make it 100% be inside the radius
					vec.x *= maxDist / dist * 0.99;
					vec.x += issuer->GetPos2D().x;
					vec.y *= maxDist / dist * 0.99;
					vec.y += issuer->GetPos2D().y;

					position->x = vec.x;
					position->y = vec.y;
					showEffects = false;
				}
			}
			break;
		}
		case DotaUnitOrder_t::DOTA_UNIT_ORDER_DROP_ITEM:
		{
			break;
		}
		case DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_NO_TARGET: {
			//Automatic mana & HP abuse with items like Arcane Boots or Faerie Fire
			static std::vector<const char*> filters = {
				"item_arcane_boots", "item_enchanted_mango",
				"item_guardian_greaves",
				"item_magic_stick",
				"item_magic_wand",
				"item_holy_locket",
				"item_soul_ring", "item_cheese", "item_arcane_ring", "item_faerie_fire", "item_greater_faerie_fire"
			};
			static std::vector<const char*> bonusTypes = {
				"bonus_int",
				"bonus_intellect",
				"bonus_strength",
				"bonus_str",
				"bonus_all_stats",
				"bonus_mana",
				"bonus_health"
			};
			if (issuer == nullptr)
				issuer = player->GetAssignedHero();

			if (!TestStringFilters(
				Interfaces::EntitySystem
				->GetEntity<BaseAbility>(abilityIndex)
				->GetIdentity()
				->GetName(),
				filters))
				break;

			BaseNpc* npc = (BaseNpc*)issuer;
			bool callPickup = false;
			uint32_t stashSlot = 6;
			for (auto& item : npc->GetItems()) {
				if (H2IDX(item.handle) == abilityIndex                   // must not be the item we're using
					|| npc->GetInventory()->GetItemSlot(item.handle) > 5 // must not bew in the backpack
					)
					continue;

				double anyBonus = 0;
				for (auto& bonus : bonusTypes) {
					anyBonus += Signatures::Scripts::GetLevelSpecialValueFor(nullptr, H2IDX(item.handle), bonus, -1);
					if (anyBonus > 0)
						break;
				}
				auto fVec = npc->GetForwardVector(5);
				if (anyBonus > 0) {
					//std::cout << abilityIndex << bonusInt << '\n';
					queue = true;
					PrepareUnitOrdersOriginal(player, DotaUnitOrder_t::DOTA_UNIT_ORDER_DROP_ITEM, 0, &fVec, H2IDX(item.handle), PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, issuer, true, false);
					callPickup = true;
				}
			}
			if (callPickup)
				// Multhithreading magic � who knows when the hero finishes dropping the items?
				manaAbusePickup = std::async(std::launch::async, [&, player, issuer]() mutable {
				Sleep(300);
			for (auto& item : physicalItems) { // wtf is with this indentation???
				if (IsWithinRadius(item->GetPos2D(), assignedHero->GetPos2D(), 50))
					PrepareUnitOrdersOriginal(player, DotaUnitOrder_t::DOTA_UNIT_ORDER_PICKUP_ITEM, H2IDX(item->GetIdentity()->entHandle), &Vector3::Zero, 0, PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, issuer, true, false);
			}
			physicalItems.clear();
					});
			break;
		}
		}

		if (giveOrder)
			PrepareUnitOrdersOriginal(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
	}

	inline bool hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID) {
#ifdef _DEBUG
		std::cout << "GCClient Send: " << std::dec << EDOTAGCMsg2String(protobufMsg->msgID) << '\n';
#endif // _DEBUG
		return BAsyncSendProtoOriginal(protobufMsg, handler, responseMsg, respMsgID);
	}

	inline bool hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket) {
#ifdef _DEBUG
		std::cout << "GCClient Recv: " << std::dec << EDOTAGCMsg2String(netPacket->GetEMsg()) << '\n';
#endif // _DEBUG
		return DispatchPacketOriginal(thisptr, netPacket);
	};

	typedef void (*PostReceivedNetMessageFn)(INetChannel*, NetMessageHandle_t*, google::protobuf::Message*, void const*, int);
	inline void hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {

		NetMessageInfo_t* info = Interfaces::NetworkMessages->GetNetMessageInfo(messageHandle);
		const char* name = info->pProtobufBinding->GetName();
		if (messageHandle->messageID == 586) { // CDOTAEntityMsg_InvokerSpellCast
			int castActivity = reinterpret_cast<VClass*>(msg)->Member<int>(0x20);

			if (castActivity == 1743) { //sunstrike
				ENT_HANDLE handle = reinterpret_cast<VClass*>(msg)
					->Member<VClass*>(0x18)
					->Member<ENT_HANDLE>(0x18);

				auto invoker = Interfaces::EntitySystem->GetEntity(handle & 0x3fff); // weird smaller mask
				if (invoker != nullptr &&
					invoker->GetTeam() != assignedHero->GetTeam())
					Modules::SunStrikeHighlighter.SunStrikeIncoming = true;

			}
		}

		return VMTs::NetChannel->GetOriginalMethod<PostReceivedNetMessageFn>(86)(thisptr, messageHandle, msg, type, bits);
	}

	typedef void* (*CreateNetChannelFn)(void*, int, void*, const char*, unsigned int, unsigned int);
	inline void* CreateNetChannel(void* thisptr, int unk, void* ns_addr, const char* str, unsigned int uUnk, unsigned int uUnk2) {
		VMTs::NetChannel.reset();
		void* ret = VMTs::NetworkSystem->GetOriginalMethod<CreateNetChannelFn>(26)(thisptr, unk, ns_addr, str, uUnk, uUnk2);

		VMTs::NetChannel = std::unique_ptr<VMT>(new VMT(ret));
		//VMTs::NetChannel->HookVM(SendNetMessage, 70);
		VMTs::NetChannel->HookVM(hkPostReceivedNetMessage, 86);
		VMTs::NetChannel->ApplyVMT();

		return ret;
	}


	inline void SetUpByteHooks() {
		if (MH_CreateHook(Signatures::PrepareUnitOrders, &hkPrepareUnitOrders,
			(LPVOID*)&PrepareUnitOrdersOriginal) != MH_OK ||
			MH_EnableHook(Signatures::PrepareUnitOrders) != MH_OK)
			std::cout << "Could not hook PrepareUnitOrders!\n";

		if (MH_CreateHook(Signatures::DispatchPacket, &hkDispatchPacket,
			(LPVOID*)&DispatchPacketOriginal) != MH_OK ||
			MH_EnableHook(Signatures::DispatchPacket) != MH_OK)
			std::cout << "Could not hook DispatchPacket!\n";

		if (MH_CreateHook(Signatures::BAsyncSendProto, &hkBAsyncSendProto,
			(LPVOID*)&BAsyncSendProtoOriginal) != MH_OK ||
			MH_EnableHook(Signatures::BAsyncSendProto) != MH_OK)
			std::cout << "Could not hook BAsyncSendProto!\n";
	}

	inline void InitVirtualHooks() {
		VMTs::NetworkSystem = std::unique_ptr<VMT>(new VMT(Interfaces::NetworkSystem));
		VMTs::NetworkSystem->HookVM(CreateNetChannel, 26);
		VMTs::NetworkSystem->ApplyVMT();
	}

}
