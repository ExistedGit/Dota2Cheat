#pragma once

#include "../SDK/CBaseFileSystem.h"

CBaseFileSystem *fs;
namespace Logger
{
    FileHandle_t logFile; // logfile Handle
}

#define MC_LOGF(f_, ...) fs->FPrintf( Logger::logFile, ("" f_), ##__VA_ARGS__);fs->Flush( Logger::logFile )