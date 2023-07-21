#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include "memory_functions.h"
using namespace std;

extern class CheatOption;
class Patch {

protected:
	PBYTE pattern = NULL;
	wstring mask;
	LPVOID originalAddress = NULL;
	PBYTE originalBytes = NULL;
	SIZE_T patchSize = NULL;
	CheatOption* parent = NULL;

	void convertPattern(LPCWSTR sign) {

		mask.clear();

		wstring signature(sign);
		wstringstream wss(signature);
		vector <BYTE> bytes;

		vector<wstring> tokens{ istream_iterator<wstring, wchar_t>(wss), {} };

		for (const wstring& str : tokens) {
			if (str.size() == 1 || str._Equal(L"xx") || str._Equal(L"XX")) {
				mask.append(L"?");
				bytes.push_back(0);
			}
			else {
				mask.append(L"x");
				BYTE singleByte = wcstoul(str.c_str(), NULL, 16);
				bytes.push_back(singleByte);
			}
		}
		if (pattern)delete[] pattern;
		pattern = new BYTE[bytes.size()];
		memcpy_s(pattern, bytes.size(), bytes.data(), bytes.size());
	}

public:
	Patch(CheatOption* parentInstance, LPCWSTR signature, int patchOffset, SIZE_T pSize) {
		parent = parentInstance;
		convertPattern(signature);
		patchSize = pSize;
	}
	Patch(CheatOption *parentInstance, LPCWSTR signature, SIZE_T pSize) :
	Patch(parentInstance, signature, NULL, pSize) {}

	virtual bool Hack(HANDLE hProcess) = 0;
	virtual bool Restore(HANDLE hProcess) = 0;
};