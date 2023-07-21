#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "Patch.h"
using namespace std;

class CheatOption
{
	//Properties
	LPCWSTR m_moduleName = NULL;
	LPCWSTR m_description = NULL;

	//Service fields
	vector <int> m_keys;
	bool m_enabled = false;
	vector <Patch*> patches;

	bool Enable(int pid);
	bool KeyPressed();

public:
	bool Disable(int pid);

	CheatOption(LPCWSTR moduleName, LPCWSTR description, const vector<int>& keys) {
		m_moduleName = moduleName;
		m_description = description;
		m_keys = keys;
		patches.clear();
	}


	CheatOption* AddNopPatch(LPCWSTR signature, SIZE_T pSize);
	CheatOption* Add—avePatch(LPCWSTR signature, PBYTE pBytes, SIZE_T patchSize);

	bool isEnabled() { return m_enabled; }
	//Disable without restor for bad exit from program
	void badDisable();
	void Process(int processId);
	LPCWSTR GetDescription() { return m_description; }
	LPCWSTR GetMoubleName() { return m_moduleName; }


};

