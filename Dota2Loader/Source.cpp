#include <iostream>
#include <ShlObj.h>

#include <Windows.h>
#include <filesystem>
#include <vector>

#include <BlackBone/Process/Process.h>
#include <BlackBone/Patterns/PatternSearch.h>
#include <BlackBone/Process/RPC/RemoteFunction.hpp>
#include <BlackBone/Syscalls/Syscall.h>

using namespace std;
using namespace blackbone;
using std::filesystem::current_path;
namespace fs = std::filesystem;

std::set<std::wstring> nativeMods, modList;

void DllLoadLibrary(Process& proc, const wstring& path) {
	auto mainThread = proc.threads().getMain();
	if (auto pLoadLibrary = MakeRemoteFunction<decltype(&LoadLibraryW)>(proc, L"Kernel32.dll", "LoadLibraryW"); pLoadLibrary && mainThread)
	{
		auto result = pLoadLibrary.Call({ path.c_str() }, mainThread);
		if (*result)
			cout << "LoadLibrary result: " << *result << "\n";
	}
}

void DllManualMap(Process& proc, const wstring& path)
{

	nativeMods.clear();
	modList.clear();

	nativeMods.emplace(L"combase.dll");
	nativeMods.emplace(L"user32.dll");
	modList.emplace(L"windows.storage.dll");
	modList.emplace(L"shell32.dll");
	modList.emplace(L"shlwapi.dll");

	auto callback = [](CallbackType type, void* /*context*/, Process& /*process*/, const ModuleData& modInfo)
	{
		if (type == PreCallback)
		{
			if (nativeMods.count(modInfo.name))
				return LoadData(MT_Native, Ldr_None);
		}
		else
		{
			if (modList.count(modInfo.name))
				return LoadData(MT_Default, Ldr_ModList);
		}

		return LoadData(MT_Default, Ldr_None);
	};

	std::wcout << L"Mapping Dota2Cheat.dll into dota2.exe..." << std::endl;
	auto image = proc.mmap().MapImage(path, NoFlags, callback);
	if (!image)
		std::wcout << L"Mapping failed with error 0x" << std::hex << image.status
		<< L". " << Utils::GetErrorDescription(image.status) << std::endl << std::endl;
	else
		std::wcout << L"Mapping successful!\n";
}
int main() {
	using namespace blackbone;

	const wstring curDir = current_path().wstring();
	const wstring injectPath = curDir + L"\\Dota2Cheat.dll";
	{
		//notepad.CreateAndAttach( L"C:\\windows\\system32\\notepad.exe", true );
		std::string userFolderPath;
		{
			char buf[256];
			SHGetSpecialFolderPathA(0, buf, CSIDL_PROFILE, false);
			userFolderPath = buf;
		}
		cout << "Updating assets...\n";
		auto cheatFolderPath = userFolderPath + "\\Documents\\Dota2Cheat";
		fs::create_directories(cheatFolderPath + "\\assets\\spellicons\\");
		fs::create_directories(cheatFolderPath + "\\scripts\\");
		fs::create_directories(cheatFolderPath + "\\config\\");
		{
			auto assetsPath = curDir;
			for (int i = 0; i < 3; i++) {
				if (fs::exists(assetsPath + L"\\assets"))
					break;
				assetsPath += L"\\..";
			}
			fs::copy(assetsPath + L"\\assets", cheatFolderPath + R"(\assets\)", fs::copy_options::recursive | fs::copy_options::skip_existing);
		}
		cout << "Updated assets\n";


		cout << "Updating scripts\n";
		{
			auto scriptsPath = curDir;
			for (int i = 0; i < 3; i++) {
				if (fs::exists(scriptsPath + L"\\scripts"))
					break;
				scriptsPath += L"\\..";
			}
			fs::copy(scriptsPath + L"\\scripts", cheatFolderPath + R"(\scripts\)", fs::copy_options::recursive | fs::copy_options::skip_existing);
		}
		cout << "Updated scripts\n";

		// Copy the local signatures just in case
		{
			auto sigPath = curDir;
			for (int i = 0; i < 3; i++) {
				if (fs::exists(sigPath + L"\\signatures.json"))
					break;
				sigPath += L"\\..";
			}
			fs::copy(sigPath + L"\\signatures.json", cheatFolderPath, fs::copy_options::overwrite_existing);
		}

		cout << "Successfully updated resources in " << cheatFolderPath << '\n';
	}

	cout << "Attaching to dota2.exe...\n";
	Process proc;
	proc.Attach(L"dota2.exe");
	if (!proc.pid())
	{
		cout << "dota2.exe is not running!\n";
		return 0;
	}

	cout << "Attached! Injecting...\n";
#ifdef _DEBUG
	DllLoadLibrary(proc, injectPath);
#else
	DllManualMap(proc, injectPath);
#endif // _DEBUG

	system("pause");
}