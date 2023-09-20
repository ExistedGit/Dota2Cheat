#pragma once
#include <vector>
#include <iostream>

// Game Events
// Since McDota's times second virtual destructors were removed

// In the Summer Patch(2023-31-08) they reworked CGameEvent's methods to use hashes(string tokens) instead of strings

class CGameEvent
{
public:
	virtual void DESTROY() = 0;

	virtual const char* GetName(void) = 0;
	virtual int GetID(void) = 0;

	virtual bool IsReliable(void) = 0;
	virtual bool IsLocal(void) = 0;
	virtual bool IsEmpty(uint32_t token) = 0;

	virtual bool GetBool(uint32_t* token, bool defaultValue = false) = 0;
	virtual int GetInt(uint32_t* token, int defaultValue = 0) = 0;
	virtual uint64_t GetUint64(uint32_t* token, uint64_t DefaultValue = 0) = 0;
	virtual float GetFloat(uint32_t* token, float defaultValue = 0.0f) = 0;
	virtual const char* GetString(uint32_t* token, char const* defaultValue = nullptr) = 0;
	virtual const void* GetPtr(uint32_t* token) = 0;

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
	virtual void SetBool(uint32_t* token, bool value) = 0;
	virtual void SetInt(uint32_t* token, int value) = 0;
	virtual void SetUint64(uint32_t* token, uint64_t value) = 0;
	virtual void SetFloat(uint32_t* token, float value) = 0;
	virtual void SetString(uint32_t* token, char const* value) = 0;
	virtual void SetPtr(uint32_t* token, void const* value) = 0;

	// Last vfunc, easier to rebuild than put up another 5 of unk()
	void* GetDataKeys() {
		return *(void**)((uintptr_t)this + 0x10);
	};
};

class IGameEventListener2
{
public:
	// rebuilt from a func in CGameEvent::GetInt callstack, you can use any other getter
	// sig for      ^^: "48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B 02 48 8B F1 48 8B CA 48 8B FA FF 50 10 3B 46 2C"
	auto GetHash(const char* cstr) {
		auto v7 = 826366252;
		auto v8 = 2i64;
		constexpr auto base = 1540483477;
		do
		{
			auto v9 = base * *(uint32_t*)cstr;
			cstr += 4;
			v7 = (base * (v9 ^ HIBYTE(v9))) ^ (base * v7);
			--v8;
		} while (v8);
		auto v10 = base
			* ((base * (v7 ^ (*(cstr + 1) << 8) ^ *cstr)) ^ ((base * (v7 ^ (*(cstr + 1) << 8) ^ (unsigned int)*cstr)) >> 13));
		return v10 ^ (v10 >> 15);
	}

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
