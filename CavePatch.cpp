#include "CavePatch.h"
#include "CheatOption.h"
#include <iostream>
#include "ldisasm.h"

PBYTE CavePatch::CalculateJumpBytes(LPVOID from, LPVOID to, BYTE* outSize)
{
	PBYTE bytes = NULL;
	LONG64 delta = reinterpret_cast<LONG64>(to) - reinterpret_cast<LONG64>(from);
	cout << "delta=" << hex << delta << endl;
	cout << "from=" << hex << reinterpret_cast<LONG64>(from) << endl;
	cout << "to=" << hex << reinterpret_cast<LONG64>(to) << endl;
	if (abs(delta) < 2147483648) {
		bytes = new BYTE[5];
		bytes[0] = 0xE9;
		delta -= 5;
		memcpy_s(bytes + 1, 4, &delta, 4);
		*outSize = 5;
	}
	else {
		bytes = new BYTE[14];
		bytes[0] = 0xFF;
		bytes[1] = 0x25;
		bytes[2] = 0x00;
		bytes[3] = 0x00;
		bytes[4] = 0x00;
		bytes[5] = 0x00;
		memcpy_s(bytes + 6, 8, &to, 8);
		*outSize = 14;
	}

	return bytes;
}

bool CavePatch::Hack(HANDLE hProcess)
{
	bool X64 = isTargetX64Process(hProcess);
	ULONG scanSize = X64 ? 0x7fffffffffffffff : 0x7fffffff;
	DWORD_PTR baseAddress = parent->GetMoubleName() && wcslen(parent->GetMoubleName()) > 0 ?
		GetModuleBaseAddress(hProcess, parent->GetMoubleName()) :
		GetProcessBaseAddress(hProcess);
	originalAddress = reinterpret_cast<LPVOID>(reinterpret_cast<PBYTE>(ScanSignature(hProcess, baseAddress, scanSize, pattern, mask)) + patchOffset);

	originalBytes = reinterpret_cast<PBYTE>(ReadMem(hProcess, originalAddress, 32));

	originalSize = NULL;

	allocatedAddress = AllocMem(hProcess, NULL, 4096);
	cout << "allocatedAddress=" << reinterpret_cast<LONG64>(allocatedAddress) << endl;
	cout <<hex<< reinterpret_cast<ULONG64>(allocatedAddress) << endl;
	BYTE jmpSize = 0;
	PBYTE jmpBytes = CalculateJumpBytes(originalAddress, allocatedAddress, &jmpSize);
	int offset = 0;

	do {
		int length = ldisasm(originalBytes + offset, X64); //может быть ошибка
		originalSize += length;
		offset += length;
	} while (originalSize < jmpSize);

	BYTE backJmpSize = 0;
	PBYTE backJmpBytes = 
		CalculateJumpBytes
		(
		reinterpret_cast<PBYTE>(allocatedAddress) + patchSize + originalSize,
		reinterpret_cast<PBYTE>(originalAddress) + jmpSize,
		&backJmpSize	
	);

	int caveSize = patchSize + originalSize + backJmpSize;
	PBYTE caveBytes = new BYTE[caveSize];
	memcpy_s(caveBytes, patchSize, patchBytes, patchSize);
	memcpy_s(caveBytes + patchSize, originalSize, originalBytes, originalSize);
	memcpy_s(caveBytes + patchSize + originalSize, backJmpSize, backJmpBytes, backJmpSize);

	WriteMem(hProcess, allocatedAddress, caveBytes, caveSize);
	delete[] caveBytes;

	int nops = originalSize - jmpSize;

	cout <<dec<< "originalSize="<< (UINT)originalSize<< " jmpSize=" << (UINT)jmpSize << " nops = " << nops << endl;
	
	PBYTE bytes = new BYTE[originalSize];
	memcpy_s(bytes, jmpSize, jmpBytes, jmpSize);
	if (nops)
		memset(bytes + jmpSize, 0x90, nops);
	WriteMem(hProcess, originalAddress, bytes, originalSize);
	delete[] bytes;


	return true;
}

bool CavePatch::Restore(HANDLE hProcess)
{
	WriteMem(hProcess, originalAddress, originalBytes, originalSize);
	FreeMem(hProcess, allocatedAddress, 4096);
	return true;
}
