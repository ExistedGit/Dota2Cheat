#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include <utility>
#include <mutex>

inline std::mutex mLogging;

enum class ConColor {
	Black,
	Blue,
	Green,
	Teal,
	Red,
	Lily,
	Yellow,
	White,
	Grey,
	LightBlue,
	LightGreen,
	LightTeal,
	LightRed,
	LightLily,
	LightYellow,
	BrightWhite
};

inline void SetConsoleColor(ConColor text = ConColor::White, ConColor background = ConColor::Black) {
	static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)(((WORD)background << 4) | (WORD)text));
};


enum LogPrefix {
	LP_NONE,
	LP_WARNING,
	LP_INFO,
	LP_ERROR,
	LP_DATA
};

inline void SetLogColor(LogPrefix prefix) {
	switch (prefix)
	{
	case LP_WARNING: SetConsoleColor(ConColor::LightYellow); break;
	case LP_INFO: SetConsoleColor(ConColor::LightTeal); break;
	case LP_ERROR: SetConsoleColor(ConColor::LightRed); break;
	case LP_DATA: SetConsoleColor(ConColor::LightGreen); break;
	default: SetConsoleColor(); break;
	}
}

inline const char* GetLogPrefix(LogPrefix prefixType) {
	switch (prefixType)
	{
	case LP_WARNING:
		return "[WARNING] ";
	case LP_INFO:
		return "[INFO] ";
	case LP_ERROR:
		return "[ERROR] ";
	case LP_DATA:
		return "[DATA] ";
	}
	return "";
}

template<typename ...Args>
void Log(LogPrefix prefixType, Args&&... args) {
	std::lock_guard<std::mutex> lk(mLogging);
	SetLogColor(prefixType);
	((std::cout << GetLogPrefix(prefixType)) << ... << args) << std::endl;
	SetConsoleColor();
}

// Formatted logging
template<typename ...Args>
void LogF(LogPrefix prefixType, std::string_view fmtString, Args&&... args) {
	std::lock_guard<std::mutex> lk(mLogging);
	SetLogColor(prefixType);
	std::cout << GetLogPrefix(prefixType) << std::vformat(fmtString, std::make_format_args(args...)) << std::endl;
	SetConsoleColor();
}

#define ADHOC_LOG(name, type) template<typename... Args> void name(Args&&... args) { Log(type, args...); };
#define ADHOC_LOGF(name, type) template<typename... Args> void name(std::string_view fmtString, Args&&... args) { LogF(type, fmtString, args...); };

ADHOC_LOG(Log, LP_NONE);
ADHOC_LOG(LogI, LP_INFO);
ADHOC_LOG(LogE, LP_ERROR);
ADHOC_LOG(LogD, LP_DATA);
ADHOC_LOG(LogW, LP_WARNING);

ADHOC_LOGF(LogF, LP_NONE);
ADHOC_LOGF(LogFI, LP_INFO);
ADHOC_LOGF(LogFE, LP_ERROR);
ADHOC_LOGF(LogFD, LP_DATA);
ADHOC_LOGF(LogFW, LP_WARNING);

#undef ADHOC_LOG
#undef ADHOC_LOGF
