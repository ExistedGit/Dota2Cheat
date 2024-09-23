#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <format>
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

inline const char* GetLogPrefix(LogPrefix prefixType) {
	switch (prefixType)
	{
	case LP_WARNING:
		SetConsoleColor(ConColor::LightYellow);
		return "[WARNING] ";
	case LP_INFO:
		SetConsoleColor(ConColor::LightTeal);
		return "[INFO] ";
	case LP_ERROR:
		SetConsoleColor(ConColor::LightRed);
		return "[ERROR] ";
	case LP_DATA:
		SetConsoleColor(ConColor::LightGreen);
		return "[DATA] ";
	}
	return "";
}

template<LogPrefix prefixType = LogPrefix::LP_NONE, typename ...Args>
void Log(Args&&... args) {
	std::lock_guard<std::mutex> lk(mLogging);
	((std::cout << GetLogPrefix(prefixType)) << ... << std::forward<Args>(args)) << '\n';
	SetConsoleColor();
}

// Formatted logging
template<LogPrefix prefixType = LogPrefix::LP_NONE, typename ...Args>
void LogF(std::string_view fmtString, Args&&... args) {
	std::lock_guard<std::mutex> lk(mLogging);
	std::cout << GetLogPrefix(prefixType) << std::vformat(fmtString, std::make_format_args(std::forward<Args>(args)...)) << '\n';
	SetConsoleColor();
}


#define ADHOC_LOG(name, type) template<typename... Args> void name(Args&&... args) { Log<type>(std::forward<Args>(args)...); };
#define ADHOC_LOGF(name, type) template<typename... Args> void name(std::string_view fmtString, Args&&... args) { LogF<type>(fmtString, std::forward<Args>(args)...); };

ADHOC_LOG(LogI, LP_INFO);
ADHOC_LOG(LogE, LP_ERROR);
ADHOC_LOG(LogD, LP_DATA);
ADHOC_LOG(LogW, LP_WARNING);

ADHOC_LOGF(LogFI, LP_INFO);
ADHOC_LOGF(LogFE, LP_ERROR);
ADHOC_LOGF(LogFD, LP_DATA);
ADHOC_LOGF(LogFW, LP_WARNING);

#undef ADHOC_LOG
#undef ADHOC_LOGF
