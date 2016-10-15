#pragma once
#include <string>

class CItemModel
{
public:
	WCHAR mType[20];
	unsigned long long mMoney;
	std::wstring mDescription;
	CItemModel();
	~CItemModel();
};

