#pragma once
#include <vector>
#include <iostream>

// Game Events
// Since McDota's times second virtual destructors were removed

// In the Summer Patch(2023-31-08) they reworked CGameEvent's methods to use hashes(string tokens) instead of strings

class CGameEvent
{
	struct EventFieldToken {
		uint32_t hash;
		const char* field;

		// reconstructed from various inlined algorithms around field names("new_state", "playerid" etc)
		// Valve did some compile-time tomfoolery and analyzing this took more than it should have
		uint32_t HashToken(const char* token);


		EventFieldToken(const char* name) : field(name) {
			hash = HashToken(name);
		}
	};

public:
	virtual void DESTROY() = 0;

	virtual const char* GetName(void) = 0;
	virtual int GetID(void) = 0;

	virtual bool IsReliable(void) = 0;
	virtual bool IsLocal(void) = 0;
	virtual bool IsEmpty(uint32_t token) = 0;

	virtual bool GetBool(const EventFieldToken& token, bool defaultValue = false) = 0;
	virtual int GetInt(const EventFieldToken& token, int defaultValue = 0) = 0;
	virtual uint64_t GetUint64(const EventFieldToken& token, uint64_t DefaultValue = 0) = 0;
	virtual float GetFloat(const EventFieldToken& token, float defaultValue = 0.0f) = 0;
	virtual const char* GetString(const EventFieldToken& token, char const* defaultValue = nullptr) = 0;
	virtual const void* GetPtr(const EventFieldToken& token) = 0;

private:
	virtual void unk0() = 0;
	virtual void unk1() = 0;
	virtual void unk2() = 0;
	virtual void unk3() = 0;
	virtual void unk4() = 0;
	virtual void unk5() = 0;
	virtual void unk6() = 0;
	virtual void unk7() = 0;

public:
	virtual void SetBool(const EventFieldToken& token, bool value) = 0;
	virtual void SetInt(const EventFieldToken& token, int value) = 0;
	virtual void SetUint64(const EventFieldToken& token, uint64_t value) = 0;
	virtual void SetFloat(const EventFieldToken& token, float value) = 0;
	virtual void SetString(const EventFieldToken& token, char const* value) = 0;
	virtual void SetPtr(const EventFieldToken& token, void const* value) = 0;

	// Last vfunc, easier to rebuild than put up another 5 of unk()
	void* GetDataKeys() {
		return *(void**)((uintptr_t)this + 0x10);
	};
};

class IGameEventListener2
{
public:
	virtual void DESTROY()
	{
	};

	virtual void FireGameEvent(CGameEvent* event) = 0;
};

class CGameEventManager
{
public:
	virtual void DESTROY() = 0;
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset(void) = 0;
	virtual bool AddListener(IGameEventListener2* listener, const char* eventName, bool serverSide = false) = 0;
	virtual bool FindListener(IGameEventListener2* listener, const char* eventName) = 0;
	virtual void RemoveListener(IGameEventListener2* listener) = 0;
};
