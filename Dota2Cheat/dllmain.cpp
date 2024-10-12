#pragma once	

#include "CheatSDK/include.h"
#include "CheatSDK/Hooking.h"
#include "CheatSDK/Debug.h"
#include "CheatSDK/MatchStateHandling.h"

#include "UI/Pages/MainMenu.h"
#include <dota_usercmd.pb.h>
#include <netmessages.pb.h>

#include "../Modules/Hacks/DotaPlusUnlocker.h"

static const char* GetWinAPIExceptionName(ULONG code) {
	switch (code) {
		CASE_STRING(STILL_ACTIVE);
		CASE_STRING(EXCEPTION_ACCESS_VIOLATION);
		CASE_STRING(EXCEPTION_DATATYPE_MISALIGNMENT);
		CASE_STRING(EXCEPTION_BREAKPOINT);
		CASE_STRING(EXCEPTION_SINGLE_STEP);
		CASE_STRING(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
		CASE_STRING(EXCEPTION_FLT_DENORMAL_OPERAND);
		CASE_STRING(EXCEPTION_FLT_DIVIDE_BY_ZERO);
		CASE_STRING(EXCEPTION_FLT_INEXACT_RESULT);
		CASE_STRING(EXCEPTION_FLT_INVALID_OPERATION);
		CASE_STRING(EXCEPTION_FLT_OVERFLOW);
		CASE_STRING(EXCEPTION_FLT_STACK_CHECK);
		CASE_STRING(EXCEPTION_FLT_UNDERFLOW);
		CASE_STRING(EXCEPTION_INT_DIVIDE_BY_ZERO);
		CASE_STRING(EXCEPTION_INT_OVERFLOW);
		CASE_STRING(EXCEPTION_PRIV_INSTRUCTION);
		CASE_STRING(EXCEPTION_IN_PAGE_ERROR);
		CASE_STRING(EXCEPTION_ILLEGAL_INSTRUCTION);
		CASE_STRING(EXCEPTION_NONCONTINUABLE_EXCEPTION);
		CASE_STRING(EXCEPTION_STACK_OVERFLOW);
		CASE_STRING(EXCEPTION_INVALID_DISPOSITION);
		CASE_STRING(EXCEPTION_GUARD_PAGE);
		CASE_STRING(EXCEPTION_INVALID_HANDLE);
		CASE_STRING(CONTROL_C_EXIT);
	}
	return nullptr;
}

extern void InGameLogic();

static LONG NTAPI VEH(PEXCEPTION_POINTERS pExceptionInfo) {
	DWORD exceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;
	PVOID exceptionAddress = pExceptionInfo->ExceptionRecord->ExceptionAddress;

	CONTEXT* ctx = pExceptionInfo->ContextRecord;
	DWORD64 rax = ctx->Rax;
	DWORD64 rbx = ctx->Rbx;
	DWORD64 rcx = ctx->Rcx;
	DWORD64 rdx = ctx->Rdx;
	DWORD64 rsi = ctx->Rsi;
	DWORD64 rdi = ctx->Rdi;
	DWORD64 rbp = ctx->Rbp;
	DWORD64 rsp = ctx->Rsp;

	std::ostringstream oss;
	oss << std::format("{} (0x{:X}) at {}", GetWinAPIExceptionName(exceptionCode), exceptionCode, exceptionAddress) << "\n";

	oss << "RAX: 0x" << std::hex << rax << "\n";
	oss << "RBX: 0x" << std::hex << rbx << "\n";
	oss << "RCX: 0x" << std::hex << rcx << "\n";
	oss << "RDX: 0x" << std::hex << rdx << "\n";
	oss << "RSI: 0x" << std::hex << rsi << "\n";
	oss << "RDI: 0x" << std::hex << rdi << "\n";
	oss << "RBP: 0x" << std::hex << rbp << "\n";
	oss << "RSP: 0x" << std::hex << rsp << "\n";

	oss << "\nCheck the console window for anything that might tell you why this happened. Screenshot any errors in case you want to report them!";

	std::string message = oss.str();

	MessageBoxA(NULL, message.c_str(), "Dota2Cheat", MB_OK | MB_ICONERROR);

	return EXCEPTION_CONTINUE_SEARCH;
}

void HackThread(HMODULE hModule) {
#ifndef _DEBUG
	AddVectoredExceptionHandler(1, VEH);
#endif

	d2c.Initialize(hModule);

	MatchStateManager.CheckForOngoingGame();

	while (!d2c.shouldUnload)
		Sleep(10);

#ifndef _DEBUG
	RemoveVectoredExceptionHandler(VEH);
#endif

	d2c.Unload();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		std::thread(&HackThread, hModule).detach();

	return TRUE;
}