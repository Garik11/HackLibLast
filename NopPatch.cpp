#include "NopPatch.h"
#include "CheatOption.h"
#include <iostream>
using namespace std;
#define NOP 0x90
bool NopPatch::Hack(HANDLE hProcess)
{
	ULONG scanSize = isTargetX64Process(hProcess) ? 0x7fffffffffffffff : 0x7fffffff;
	GetProcessBaseAddress(hProcess);
	DWORD_PTR baseAddress = parent->GetMoubleName() && wcslen(parent->GetMoubleName()) > 0 ?
		GetModuleBaseAddress(hProcess, parent->GetMoubleName()) :
		GetProcessBaseAddress(hProcess);
	cout << hex << GetProcessBaseAddress(hProcess) << endl;
	originalAddress = ScanSignature(hProcess, baseAddress, scanSize, pattern, mask);
	originalBytes = reinterpret_cast<PBYTE>(ReadMem(hProcess, originalAddress, patchSize));
	cout <<"originalAddress=" << reinterpret_cast<ULONG64>(originalAddress) << endl;
	PBYTE patchBytes = new BYTE[patchSize];
	memset(patchBytes, 0x90, patchSize);
	WriteMem(hProcess, originalAddress, patchBytes, patchSize);
	delete[] patchBytes;

	return true;
}

bool NopPatch::Restore(HANDLE hProcess)
{
	WriteMem(hProcess, originalAddress, originalBytes, patchSize);
	return false;
}
