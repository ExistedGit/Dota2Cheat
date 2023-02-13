#include <iostream>
#include <Windows.h>
#include <filesystem>

using namespace std;
using std::filesystem::current_path;

int main() {
	const string curDir = current_path().string();
	const string path = (curDir + "\\Dota2Cheat.dll");
	LPCSTR DllPath = path.c_str();
	auto handle = FindWindow(nullptr, L"Dota 2");
	if (!handle) {
		cout << "Dota 2 is not running!";
		system("pause");
		return 0;
	}

	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);



	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath,
		strlen(DllPath) + 1, 0);

	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);

	WaitForSingleObject(hLoadThread, INFINITE);

	VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);

	cout << "Injection successful.\n";
	system("pause");
}