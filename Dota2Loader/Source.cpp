#include <iostream>
#include <Windows.h>
#include <string>
#include <memory>
#include <TlHelp32.h>


DWORD GetProcessIdByName(const wchar_t* const processName)
{
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32W processEntry{};
		processEntry.dwSize = sizeof(PROCESSENTRY32W);

		if (Process32FirstW(hSnapshot, &processEntry))
		{
			do
			{
				if (_wcsicmp(processEntry.szExeFile, processName) == 0)
				{
					processId = processEntry.th32ProcessID;
					break;
				}
			} while (Process32NextW(hSnapshot, &processEntry));
		}

		CloseHandle(hSnapshot);
	}

	return processId;
}

bool OpenProcessWithRights(DWORD processId, HANDLE& hProcess)
{
	hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
	if (!hProcess)
	{
		std::cout << "Ошибка при открытии процесса. Код ошибки: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool AllocateMemoryInProcess(HANDLE hProcess, const wchar_t* const dllPath, LPVOID& dllPathRemote)
{
	size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);

	dllPathRemote = VirtualAllocEx(hProcess, nullptr, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!dllPathRemote)
	{
		std::cout << "Ошибка при выделении памяти в процессе. Код ошибки: " << GetLastError() << std::endl;
		return false;
	}

	if (!WriteProcessMemory(hProcess, dllPathRemote, dllPath, dllPathSize, nullptr))
	{
		std::cout << "Ошибка при записи пути к DLL в память процесса. Код ошибки: " << GetLastError() << std::endl;
		VirtualFreeEx(hProcess, dllPathRemote, 0, MEM_RELEASE); // Освобождаем память в случае ошибки записи
		return false;
	}

	return true;
}

bool CreateRemoteThreadForLoadLibrary(HANDLE hProcess, HANDLE& hRemoteThread, const wchar_t* const dllPath)
{
	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	if (!hKernel32)
	{
		std::cout << "Ошибка при получении адреса kernel32.dll. Код ошибки: " << GetLastError() << std::endl;
		return false;
	}

	FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryW");
	if (!loadLibraryAddr)
	{
		std::cout << "Ошибка при получении адреса функции LoadLibraryW. Код ошибки: " << GetLastError() << std::endl;
		return false;
	}

	hRemoteThread = CreateRemoteThread(hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryAddr), const_cast<wchar_t*>(dllPath), 0, nullptr);
	if (!hRemoteThread)
	{
		std::cout << "Ошибка при создании удаленного потока. Код ошибки: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool WaitForThreadCompletion(HANDLE hRemoteThread)
{
	if (WaitForSingleObject(hRemoteThread, INFINITE) == WAIT_FAILED)
	{
		std::cout << "Ошибка при ожидании завершения удаленного потока. Код ошибки: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool InjectDll(DWORD processId, const wchar_t* const dllPath)
{
	HANDLE hProcess;
	if (!OpenProcessWithRights(processId, hProcess))
		return false;

	std::unique_ptr<void, decltype(&CloseHandle)> hProcessHandle(hProcess, &CloseHandle);

	LPVOID dllPathRemote = nullptr;
	if (!AllocateMemoryInProcess(hProcess, dllPath, dllPathRemote))
		return false;

	HANDLE hRemoteThread;
	if (!CreateRemoteThreadForLoadLibrary(hProcess, hRemoteThread, reinterpret_cast<const wchar_t*>(dllPathRemote)))
		return false;

	if (!WaitForThreadCompletion(hRemoteThread))
		return false;

	return true;
}

int main()
{
	const char* const processName = "dota2.exe"; // Имя процесса Dota 2
	const wchar_t* const dllPath = L"C:\\Users\\Wh3baby\\Desktop\\Dota2Cheat.dll"; // Путь к DLL для инъекции // "C:\Users\Wh3baby\Desktop\Dota2Cheat.dll"
	wchar_t wideProcessName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, processName, -1, wideProcessName, MAX_PATH);

	DWORD processId = GetProcessIdByName(wideProcessName);
	if (processId != 0)
	{
		if (InjectDll(processId, dllPath))
		{
			std::cout << "DLL успешно инжектирована!" << std::endl;
		}
		else
		{
			std::cout << "Ошибка при инжектировании DLL." << std::endl;
		}
	}
	else
	{
		std::cout << "Процесс Dota 2 не найден." << std::endl;
	}

	return 0;
}