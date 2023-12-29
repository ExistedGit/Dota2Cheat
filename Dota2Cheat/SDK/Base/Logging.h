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

template<typename ...Args>
void Log(LogPrefix prefixType, Args&&... args) {
//#ifndef _DEBUG
//	if (prefixType != LP_ERROR && prefixType != LP_WARNING)
//		return;
//#endif
	std::lock_guard<std::mutex> lk(mLogging);
	std::string prefix = GetLogPrefix(prefixType);
	((std::cout << prefix) << ... << args) << '\n';
	SetConsoleColor();
}

template<typename ...Args>
void LogI(Args&&... args) {
//#ifndef _DEBUG
//	return;
//#endif
	std::lock_guard<std::mutex> lk(mLogging);
	std::string prefix = GetLogPrefix(LP_INFO);
	((std::cout << prefix) << ... << args) << '\n';
	SetConsoleColor();
}

// Formatted log
template<typename ...Args>
void LogF(LogPrefix prefixType, const char* fmtString, Args&&... args) {
//#ifndef _DEBUG
//	if (prefixType != LP_ERROR && prefixType != LP_WARNING)
//		return;
//#endif
	std::lock_guard<std::mutex> lk(mLogging);
	std::string prefix = GetLogPrefix(prefixType);
	std::cout << prefix << std::vformat(fmtString, std::make_format_args(std::forward<Args>(args)...)) << '\n';
	SetConsoleColor();
}