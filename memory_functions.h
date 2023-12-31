#pragma once
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include <sstream>
#include <psapi.h>
#include <iostream>
using namespace std;

int GetProcessIdByWindowName(LPCWSTR className, LPCWSTR windowName);
int GetProcessIdByProcessName(LPCWSTR processName);
int WriteMem(HANDLE hProcess, LPVOID address, LPVOID source, SIZE_T writeAmount);
LPVOID ReadMem(HANDLE hProcess, LPVOID address, SIZE_T readAmount);
LPVOID AllocMem(HANDLE hProcess, LPVOID startAddress, SIZE_T allocationAmount);
int FreeMem(HANDLE hProcess, LPVOID address, SIZE_T amount);
LPVOID ScanSignature(HANDLE hProcess, ULONG_PTR startAddress, SIZE_T scanSize, PBYTE pattern, wstring& mask);
bool CheckSignature(PBYTE source, PBYTE pattern, wstring& mask);
void ShowErrorMessage(HWND hwnd, LPCWSTR errorMessage);
DWORD_PTR GetProcessBaseAddress(HANDLE hProcess);
DWORD_PTR GetModuleBaseAddress(HANDLE hProcess, LPCWSTR lpszModuleName);

int GetProcessIdByWindowName(LPCWSTR className, LPCWSTR windowName)
{
	HWND window = FindWindow(className, windowName);
	int pid = 0;
	GetWindowThreadProcessId(window, reinterpret_cast<LPDWORD>(&pid));
	return pid;
}

int GetProcessIdByProcessName(LPCWSTR processName)
{
	int pid = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (!snapshot) {
		ShowErrorMessage(NULL, L"Snapshot creation with error");
		return 0;
	}
	PROCESSENTRY32W pe = {};
	pe.dwSize = sizeof(PROCESSENTRY32W);
	if (Process32First(snapshot, &pe)) {
		do {
			if (wcscmp(processName, pe.szExeFile) == 0) {
				pid = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &pe));
	}
	CloseHandle(snapshot);
	return pid;
}

DWORD_PTR GetProcessBaseAddress(HANDLE hProcess) {
	DWORD_PTR baseAddress = NULL;
	HMODULE* moduleArray = NULL;
	LPBYTE moduleArrayBytes = NULL;
	DWORD bytesRequired = NULL;
	EnumProcessModules(hProcess, NULL, NULL, &bytesRequired);
	if (!EnumProcessModules(hProcess, NULL, NULL, &bytesRequired) || !bytesRequired) return NULL;

	moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

	if (moduleArrayBytes)
	{
		unsigned int moduleCount;
		
		moduleCount = bytesRequired / sizeof(HMODULE);
		moduleArray = (HMODULE*)moduleArrayBytes;

		if (EnumProcessModules(hProcess, moduleArray, bytesRequired, &bytesRequired))
		{
			baseAddress = (DWORD_PTR)moduleArray[0];
		}
		LocalFree(moduleArrayBytes);
	}
	return baseAddress;
}

DWORD_PTR GetModuleBaseAddress(HANDLE hProcess, LPCWSTR lpszModuleName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(hProcess));
	DWORD dwModuleBaseAddress = 0;
	if (hSnapshot != INVALID_HANDLE_VALUE) 
	{
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &ModuleEntry32)) 
		{
			do {
				if (wcscmp(ModuleEntry32.szExePath, lpszModuleName) == 0)
				{
					dwModuleBaseAddress = reinterpret_cast<DWORD_PTR>(ModuleEntry32.modBaseAddr);
					break;
				}

			} while(Module32Next(hSnapshot, &ModuleEntry32));
		}
		CloseHandle(hSnapshot);
	}
	return dwModuleBaseAddress;
}

int WriteMem(HANDLE hProcess, LPVOID address, LPVOID source, SIZE_T writeAmount)
{
	if (!hProcess) {
		ShowErrorMessage(NULL, L"Failed to open process");
		return -1;
	}

	SIZE_T bytesWrite = 0;
	DWORD oldProtect = 0;

	VirtualProtectEx(hProcess, address, writeAmount, PAGE_READWRITE, &oldProtect);

	WriteProcessMemory(hProcess, address, source, writeAmount, &bytesWrite);
	if (!bytesWrite) {
		ShowErrorMessage(NULL, L"Write memory faild");
	}

	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(address), writeAmount, oldProtect, &oldProtect);
	return 0;
}

LPVOID ReadMem(HANDLE hProcess, LPVOID address, SIZE_T readAmount)
{
	if (!hProcess) {
		ShowErrorMessage(NULL, L"Failed to open process");
		return nullptr;
	}

	SIZE_T bytesRead = 0;

	unsigned char* buf = new unsigned char[readAmount];
	memset(buf, 0, readAmount);

	ReadProcessMemory(hProcess, address, buf, readAmount, &bytesRead);
	if (!bytesRead) {
		ShowErrorMessage(NULL, L"Read memory faild");
	}

	return reinterpret_cast<LPVOID>(buf);
}

LPVOID AllocMem(HANDLE hProcess, LPVOID startAddress, SIZE_T allocationAmount)
{
	if (!hProcess) {
		ShowErrorMessage(NULL, L"Failed to open process");
		return nullptr;
	}

	int bytesWrite = 0;

	LPVOID ptr = VirtualAllocEx(hProcess, startAddress, allocationAmount, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!ptr) {
		ShowErrorMessage(NULL, L"Allocation faild");
	}
	return ptr;
}

int FreeMem(HANDLE hProcess, LPVOID address, SIZE_T amount)
{
	if (!hProcess) {
		ShowErrorMessage(NULL, L"Failed to open process");
		return -1;
	}

	int bytesWrite = 0;

	bool result = VirtualFreeEx(hProcess, address, amount, MEM_DECOMMIT);

	if (!result) {
		ShowErrorMessage(NULL, L"Free memory faild");
	}

	return 0;
}

LPVOID ScanSignature(HANDLE hProcess, ULONG_PTR startAddress, SIZE_T scanSize, PBYTE pattern, wstring& mask)
{
	ULONG64 result = 0;
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	DWORD offset = 0;
	SIZE_T bytesRead = 0;

	if (!hProcess)return reinterpret_cast<LPVOID>(result);
	while (offset < (scanSize - mask.size()) && result == 0) {
		int count = VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(startAddress + offset), &mbi, sizeof(mbi));
		if (!count) break;
		if (mbi.State != MEM_FREE) {
			BYTE* buffer = new BYTE[mbi.RegionSize];
			void* baseAddress = mbi.BaseAddress;
			ReadProcessMemory(hProcess, baseAddress, buffer, mbi.RegionSize, &bytesRead);
			if (bytesRead == 0) break;
			for (int i = 0; i < (mbi.RegionSize + mask.size()); i++) {
				if (CheckSignature(buffer + i, pattern, mask)) {
					result = startAddress + offset + i;
					break;
				}
			}
			delete[] buffer;
		}
		offset += mbi.RegionSize;
	}
	return reinterpret_cast<LPVOID>(result);
}

bool CheckSignature(PBYTE source, PBYTE pattern, wstring& mask)
{
	for (int i = 0; i < mask.size(); i++) {
		if (mask.at(i) == L'?' || (*(source + i) == *(pattern + i)))
			continue;
		else
			return false;
	}
	return true;
}

void ShowErrorMessage(HWND hwnd, LPCWSTR errorMessage)
{
	int err = GetLastError();
	wstring errStr(errorMessage);
	errStr += L"\r\nError code:" + err;
	MessageBox(NULL, errStr.c_str(), L"ERROR", MB_OK | MB_ICONERROR);
}

bool isTargetX64Process(HANDLE hProcess) {
	if (hProcess) {
		USHORT pProc = 0;
		USHORT pNative = 0;

		IsWow64Process2(hProcess, &pProc, &pNative);
		return !pProc;
	}
	return false;
}