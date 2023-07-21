#pragma once
#include "Patch.h"

class CavePatch : public Patch {
	LPVOID allocatedAddress = NULL;
	PBYTE patchBytes = NULL;
	BYTE originalSize = NULL;
	int caveSize = NULL;
	int patchOffset = 0; //может быть ошибка

	PBYTE CalculateJumpBytes(LPVOID from, LPVOID to, BYTE* outSize);

public:
	CavePatch(CheatOption* parentInstance, LPCWSTR signature, PBYTE pBytes, SIZE_T pSize):
		Patch(parentInstance, signature, pSize)
	{
		patchBytes = pBytes;
	}

	bool Hack(HANDLE hProcess);
	bool Restore(HANDLE hProcess);

};