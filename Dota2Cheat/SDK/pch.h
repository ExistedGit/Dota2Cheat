#define _CRT_SECURE_NO_WARNINGS
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
#include <map>
#include <vector>
#include <set>

#include <stdio.h>
#include <fstream>
#include <sstream> 

#include "Base/Definitions.h"
#include "Base/Bytehook.h"
#include "Base/VClass.h"
#include "Base/NormalClass.h"
#include "Base/StringUtils.h"
#include "Base/Logging.h"

#include "Enums.h"

#include "Entities/CDOTABaseNPCHero.h"
#include "Entities/CDOTAPlayerController.h"
#include "Entities/CDOTAModifier.h"
#include "Entities/CDOTAItem.h"

#include "Globals/Context.h"
#include "Globals/GameSystems.h"
#include "Globals/Interfaces.h"
#include "Globals/Signatures.h"

#include "Wrappers/Creep.h"
#include "Wrappers/EntitySorting.h"
#include "Wrappers/MapThreadSafe.h"

#include "Protobufs/dota_clientmessages.pb.h"
#include "Protobufs/dota_commonmessages.pb.h"
#include "Protobufs/dota_gcmessages_msgid.pb.h"
#include "Protobufs/dota_gcmessages_common.pb.h"
#include "Protobufs/dota_shared_enums.pb.h"
#include "Protobufs/dota_usermessages.pb.h"
#include "Protobufs/econ_gcmessages.pb.h"
#include "Protobufs/econ_shared_enums.pb.h"
#include "Protobufs/gcsdk_gcmessages.pb.h"
#include "Protobufs/netmessages.pb.h"
#include "Protobufs/networkbasetypes.pb.h"
#include "Protobufs/network_connection.pb.h"
#include "Protobufs/steammessages.pb.h"
#include "Protobufs/steammessages_steamlearn.steamworkssdk.pb.h"
#include "Protobufs/steammessages_unified_base.steamworkssdk.pb.h"
#include "Protobufs/usermessages.pb.h"

#endif // !PCH_H