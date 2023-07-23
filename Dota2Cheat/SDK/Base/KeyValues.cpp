#include "KeyValues.h"
#include "../../CheatSDK/Globals/include.h"

bool KeyValues::LoadFromFile(const char* path) {
	auto func = Memory::GetExport("tier0.dll", "?LoadFromFile@KeyValues@@QEAA_NPEAVIFileSystem@@PEBD1P6A_N1PEAX@Z21@Z");
	return func.Call<bool>(this, Interfaces::FileSystem, path, "GAME", nullptr, nullptr, nullptr);
}
