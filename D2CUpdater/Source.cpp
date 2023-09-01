#include <iostream>
#include <ShlObj.h>

#include <Windows.h>
#include <filesystem>
#include <vector>
#include "../Dota2Cheat/SDK/Base/Logging.h"

using namespace std;
using std::filesystem::current_path;
namespace fs = std::filesystem;

int main() {
	const wstring curDir = current_path().wstring();

	std::string userFolderPath;
	{
		char buf[256];
		SHGetSpecialFolderPathA(0, buf, CSIDL_PROFILE, false);
		userFolderPath = buf;
	}
	cout << "Updating assets...\n";

	auto cheatFolderPath = userFolderPath + "\\Documents\\Dota2Cheat";
	fs::create_directories(cheatFolderPath + "\\assets\\spellicons\\");
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

	SetConsoleColor(ConColor::Green);
	cout << "Successfully updated resources in " << cheatFolderPath << '\n';
	SetConsoleColor();

	Sleep(5000);
}