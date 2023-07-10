#pragma once
#include "../SDK/pch.h"
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

struct IM_EntityListener {
	virtual void OnEntityAdded(const EntityWrapper& ent) {};
	virtual void OnEntityRemoved(const EntityWrapper& ent) {};
};

inline class CEntityList : public IEntityListener {
	qwemap<uint32_t, EntityWrapper> Entities;

	std::vector<IM_EntityListener*> Listeners;

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

		EntityWrapper wrap;

		using enum EntityType;
		switch (CityHash32(className)) {
		case "C_DOTA_Item_Physical"_city32: wrap.type = PhysicalItem; break;
		case "C_DOTA_Item_Rune"_city32: wrap.type = Rune; break;
		}

		if (wrap.type != Undefined)
			if (className.starts_with("C_DOTA_Unit_Hero")
				|| className.starts_with("CDOTA_Unit_Hero"))
				wrap.type = Hero;
			else if (className.find("Creep") != -1)
				wrap.type = Creep;

		if (wrap.type == Undefined)
			return;


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

	void AddListener(IM_EntityListener& l) {
		Listeners.push_back(&l);
	}
	template<size_t lSize>
	void AddListeners(IM_EntityListener* const (&_listeners)[lSize]) {
		for (auto l : _listeners)
			Listeners.push_back(l);
	}

	template<typename ...Args>
	bool ContainsTypes(std::function<bool(const EntityWrapper&)> predicate, Args&&... _types) {
		std::lock_guard<std::mutex> lock(mEntities);

		EntityType types[] = { std::forward<Args>(_types) };
		
		for (const auto& [idx, wrap] : Entities) {
			bool proceed = false;
			for (auto type : types)
				if (wrap.type == type) {
					proceed = true; break;
				}

			if (proceed)
				return true;
		}

		return false;
	}

	template<typename ...Args>
	std::vector<EntityWrapper> FilterByType(std::function<bool(const EntityWrapper&)> predicate, Args&&... _types) {
		std::lock_guard<std::mutex> lock(mEntities);

		EntityType types[] = { std::forward<Args>(_types) };
		std::vector<EntityWrapper> ret;

		for (const auto& [idx, wrap] : Entities) {
			bool proceed = false;
			for (auto type : types)
				if (wrap.type == type) {
					proceed = true; break;
				}

			if (proceed && predicate(wrap))
				ret.push_back(wrap);
		}

		return ret;
	}

	std::vector<EntityWrapper> Filter(std::function<bool(const EntityWrapper&)> predicate) {
		std::lock_guard<std::mutex> lock(mEntities);
		std::vector<EntityWrapper> ret;

		for (const auto& [idx, wrap] : Entities)
			if (predicate(wrap))
				ret.push_back(wrap);

		return ret;
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

} EntityList;