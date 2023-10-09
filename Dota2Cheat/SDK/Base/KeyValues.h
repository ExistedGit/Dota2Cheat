#pragma once
#include "Memory.h"
#include "MemAlloc.h"

struct KeyValues {
	// from TF2 source
	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_COMPILED_INT_BYTE,			// hack to collapse 1 byte ints in the compiled format
		TYPE_COMPILED_INT_0,			// hack to collapse 0 in the compiled format
		TYPE_COMPILED_INT_1,			// hack to collapse 1 in the compiled format
		TYPE_NUMTYPES,
	};

	types_t GetDataType(const char* key = nullptr) const {
		auto func = Memory::GetExport("tier0.dll", "?GetDataType@KeyValues@@QEBA?AW4types_t@CKeyValues_Data@@PEBD@Z");
		return func.Call<types_t>(this, key);
	}

	static KeyValues* MakeKV(const char* name) {
		auto kv = CMemAlloc::Instance()->Alloc<KeyValues>(0x14);
		auto ctor = Memory::GetExport("tier0.dll", "??0KeyValues@@QEAA@PEBD@Z");
		ctor(kv, name);
		return kv;
	}

	bool LoadFromFile(const char* path);

	KeyValues* GetFirstSubKey() const {
		auto func = Memory::GetExport("tier0.dll", "?GetFirstSubKey@KeyValues@@QEBAPEAV1@XZ");
		return func.Call<KeyValues*>(this);
	}

	KeyValues* GetNextKey() const {
		auto func = Memory::GetExport("tier0.dll", "?GetNextKey@KeyValues@@QEBAPEAV1@XZ");
		return func.Call<KeyValues*>(this);
	}

	const char* GetName() {
		auto func = Memory::GetExport("tier0.dll", "?GetName@KeyValues@@QEBAPEBDXZ");
		return func.Call<const char*>(this);
	}

	void Destroy() {
		auto func = Memory::GetExport("tier0.dll", "??1KeyValues@@QEAA@XZ");
		func(this);
	}

	struct KVIterator {
		KeyValues* node;

		KVIterator(KeyValues* node) : node(node) {}

		bool operator!=(const KVIterator& other) {
			return other.node != node;
		}

		KeyValues* operator*() const {
			return node;
		}

		KVIterator operator++() {
			node = node->GetNextKey();
			return *this;
		}
	};


	KVIterator begin() const {
		return GetFirstSubKey();
	}

	KVIterator end() const {
		return nullptr;
	}
};

