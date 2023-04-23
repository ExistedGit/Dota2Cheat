#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <format>

inline HANDLE hStdOut = nullptr;

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
	if (!hStdOut)
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)(((WORD)background << 4) | (WORD)text));
};


enum LogPrefix {
	LP_NONE,
	LP_WARNING,
	LP_INFO,
	LP_ERROR,
	LP_DATA
};

inline std::string GetLogPrefix(LogPrefix prefixType) {
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

#ifdef _DEBUG
template<typename ...Args>
void Log(LogPrefix prefixType, Args&&... args) {
	std::string prefix = GetLogPrefix(prefixType);
	((std::cout << prefix) << ... << args) << '\n';
	SetConsoleColor();
}
#else
#define Log(...)
#endif
#ifdef _DEBUG
// Formatted log
template<typename ...Args>
void LogF(LogPrefix prefixType, const char* fmtString, Args&&... args) {
	std::string prefix = GetLogPrefix(prefixType);
	std::cout << prefix << std::vformat(fmtString, std::make_format_args(std::forward<Args>(args)...)) << '\n';
	SetConsoleColor();
}
#else
#define LogF(...)
#endif