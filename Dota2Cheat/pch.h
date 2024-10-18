#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <memory>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <malloc.h>
#include <locale.h>
#include <math.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <string_view>
#include <span>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <format>
#include <algorithm>

#include <nlohmann/json.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include "SDK/include.h"

#include "CheatSDK/Bytes/Fonts.h"

using namespace std::string_view_literals;
using namespace std::string_literals;

#endif // !PCH_H