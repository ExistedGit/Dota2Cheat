#include "CGameEventManager.h"

// reconstructed from various inlined algorithms around field names("new_state", "playerid" etc)
// Valve did some compile-time tomfoolery and analyzing this took more than it should have

uint32_t CGameEvent::EventFieldToken::HashToken(const char* token) {
	auto tokenSize = strlen(token);
	auto v11 = 0x3141592E + tokenSize % 4;

	uint64_t v12 = strlen(token) / 4;
	auto seed = 1540483477;
	uint32_t v22 = 0;
	do
	{
		auto v21 = seed * *(uint32_t*)token;
		token += 4;
		v22 = (seed * v11) ^ (seed * (v21 ^ (v21 >> 0x18)));
		v11 = v22;
		--v12;
	} while (v12);

	auto v15 = v22;
	for (int i = 0; i < strlen(token) % 4; i++) {
		auto oddChar = *(token + i);

		if (i != 0)
			oddChar = oddChar << (2 << (i + 1));

		v15 ^= oddChar;
	}

	if (strlen(token) % 4 > 0)
		v15 *= seed;

	auto v23 = seed * (v15 ^ (v15 >> 13));

	return (v23 ^ (v23 >> 15));

}
