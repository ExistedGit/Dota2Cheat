#pragma once
#include "../../pch.h"
#include "../Globals/include.h"
#include "../MapThreadSafe.h"
#include <functional>
#include <mutex>
#include <consthash/cityhash32.hxx>
#include <cityhash/city.h>

enum class CreepType {
	NotCreep,
	LaneMelee,
	LaneRanged,
	Siege
};

enum class EntityType {
	Undefined,
	Hero,
	Creep,
	Rune,
	PhysicalItem,
};

struct EntityWrapper {
	CBaseEntity* ent;
	EntityType type = EntityType::Undefined;
	CreepType creepType = CreepType::NotCreep;

	CBaseEntity* operator->() const {
		return ent;
	}

	template<typename T>
	operator T* () const {
		return (T*)ent;
	}

	template<typename T>
	T* As() const {
		return (T*)ent;
	}
};

struct IEntityListListener {
	virtual void OnEntityAdded(const EntityWrapper& ent) {};
	virtual void OnEntityRemoved(const EntityWrapper& ent) {};
};

inline class CEntityList : public IEntityListener {
	qwemap<uint32_t, EntityWrapper> Entities;

	std::vector<IEntityListListener*> Listeners;

	void DispatchEntityAdded(const EntityWrapper& ent) {
		for (auto l : Listeners)
			l->OnEntityAdded(ent);
	}

	void DispatchEntityRemoved(const EntityWrapper& ent) {
		for (auto l : Listeners)
			l->OnEntityRemoved(ent);
	}

public:
	std::mutex mEntities;

	void OnEntityCreated(CBaseEntity* ent) override {
		std::lock_guard<std::mutex> lock(mEntities);
		if (!ent->SchemaBinding()->binaryName || Entities.contains(ent->GetIndex()))
			return;

		std::string_view className = ent->SchemaBinding()->binaryName;
		if (className == "C_DOTAGamerulesProxy")
			GameSystems::GameRules = ent->Member<CDOTAGameRules*>(Netvars::C_DOTAGamerulesProxy::m_pGameRules);


		using enum EntityType;
		EntityType entType = Undefined;
		switch (CityHash32(className)) {
		case "C_DOTA_Item_Physical"_city32: entType = PhysicalItem; break;
		case "C_DOTA_Item_Rune"_city32: entType = Rune; break;
		}

		if (entType == Undefined)
			if (className.starts_with("C_DOTA_Unit_Hero")
				|| className.starts_with("CDOTA_Unit_Hero"))
				entType = Hero;
			else if (className.find("Creep") != -1)
				entType = Creep;

		if (entType == Undefined)
			return;

		EntityWrapper wrap{
			.ent = ent,
			.type = entType
		};

		if (wrap.type == Creep) {
			if (className == "C_DOTA_BaseNPC_Creep_Siege")
				wrap.creepType = CreepType::Siege;
			else if (className == "C_DOTA_BaseNPC_Creep_Lane") {
				if (((CDOTABaseNPC*)ent)->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK)
					wrap.creepType = CreepType::LaneMelee;
				else
					wrap.creepType = CreepType::LaneRanged;
			}
		}
		Entities[ent->GetIndex()] = wrap;
		DispatchEntityAdded(wrap);
	}

	void OnEntityDeleted(CBaseEntity* ent) override {
		std::lock_guard<std::mutex> lock(mEntities);
		if (Entities.contains(ent->GetIndex())) {
			DispatchEntityRemoved(Entities[ent->GetIndex()]);
			Entities.erase(ent->GetIndex());
		}
	}

	void AddListener(IEntityListListener& l) {
		Listeners.push_back(&l);
	}
	template<size_t lSize>
	void AddListeners(IEntityListListener* const (&_listeners)[lSize]) {
		for (auto l : _listeners)
			Listeners.push_back(l);
	}



	template<typename ...Args>
	bool ContainsTypes(std::function<bool(const EntityWrapper&)> predicate, Args&&... _types) {
		std::lock_guard<std::mutex> lock(mEntities);

		EntityType types[] = { std::forward<Args>(_types)... };

		for (const auto& [idx, wrap] : Entities) {
			bool proceed = false;
			for (auto type : types)
				if (wrap.type == type) {
					proceed = true; break;
				}

			if (proceed && predicate(wrap))
				return true;
		}

		return false;
	}

	CBaseEntity* Find(std::function<bool(const EntityWrapper&)> predicate) {
		std::lock_guard<std::mutex> lock(mEntities);

		for (const auto& [idx, wrap] : Entities)
			if (predicate(wrap))
				return wrap;

		return nullptr;
	}
	template<typename T>
	T* Find(std::function<bool(T*)> predicate) {
		std::lock_guard<std::mutex> lock(mEntities);

		EntityType type = EntityType::Undefined;

		if constexpr (std::is_same_v<T, CDOTABaseNPC_Hero>)
			type = EntityType::Hero;

		if (type == EntityType::Undefined)
			return nullptr;

		for (const auto& [idx, wrap] : Entities)
			if (wrap.type == type && predicate(wrap))
				return wrap;

		return nullptr;
	}

	void ForEach(std::function<void(EntityWrapper&)> func) {
		std::lock_guard<std::mutex> lock(mEntities);
		for (auto& [idx, wrap] : Entities)
			func(wrap);
	}

	void ForEachOfType(EntityType type, std::function<void(EntityWrapper&)> func) {
		std::lock_guard<std::mutex> lock(mEntities);
		for (auto& [idx, wrap] : Entities)
			if (wrap.type == type)
				func(wrap);
	}
	template<typename T>
	void ForEach(std::function<void(T*)> func) {
		std::lock_guard<std::mutex> lock(mEntities);

		EntityType type = EntityType::Undefined;

		if constexpr (std::is_same_v<T, CDOTABaseNPC_Hero>)
			type = EntityType::Hero;

		if (type == EntityType::Undefined)
			return;

		for (auto& [idx, wrap] : Entities)
			if (wrap.type == type)
				func(wrap);
	}

	bool IsEntityOfType(CBaseEntity* ent, EntityType type) {
		return Entities.contains(ent->GetIndex()) && Entities[ent->GetIndex()].type == type;
	}

#define ISOFTYPE_TEMPLATE(name, type) bool name(CBaseEntity* ent) { \
										return IsEntityOfType(ent, type);\
									  }
	ISOFTYPE_TEMPLATE(IsHero, EntityType::Hero);
#undef ISOFTYPE_TEMPLATE

} EntityList;