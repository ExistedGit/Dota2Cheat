#include "Signatures.h"

void Signatures::FindSignatures() {
	CMsg = Memory::GetExport("tier0.dll", "Msg");
	CMsgColor = Memory::GetExport("tier0.dll", "?ConColorMsg@@YAXAEBVColor@@PEBDZZ");

	SignatureDB::ParseSignatures(NamedSignatures);
}
