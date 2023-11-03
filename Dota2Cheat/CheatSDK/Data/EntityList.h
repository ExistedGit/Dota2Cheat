#pragma once
#include "../../pch.h"
#include "../Globals/include.h"
#include <functional>
#include <mutex>

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
	CBaseEntity* ent{};
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
	std::map<uint32_t, EntityWrapper> Entities;

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

	void OnEntityCreated(CBaseEntity* ent) override;

	void OnEntityDeleted(CBaseEntity* ent) override;

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

	bool ContainsType(EntityType type) {
		std::lock_guard<std::mutex> lock(mEntities);

		for (const auto& [idx, wrap] : Entities) {
			if (wrap.type == type)
				return true;
		}
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