#include "C_DOTAGameRules.h"

float CDOTAGameRules::GetGameTime() {
	float result = 0;
	GetGameTimeFunc(&result, 0);
	return result;
}

std::vector<ItemStockInfo*> CDOTAGameRules::GetItemStockInfo() {
	CUtlVector<ItemStockInfo>* arr = (CUtlVector<ItemStockInfo>*)((uintptr_t)this + Netvars::C_DOTAGamerules::m_vecItemStockInfo);
	auto result = std::vector<ItemStockInfo*>{};
	result.reserve(arr->m_Size);
	for (int i = 0; i < arr->m_Size; i++)
		result.push_back(&arr->at(i));

	return result;
}
