#include "CUIPanel.h"
#include "CUIEngineSource2.h"

void CUIPanel::AddClass(const char* class_) {
	CallVFunc<137>(CUIEngineSource2::Get()->MakeSymbol(class_));
}

void CUIPanel::RemoveClass(const char* class_) {
	RemoveClass(CUIEngineSource2::Get()->MakeSymbol(class_));
}
