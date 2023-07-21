#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <map>
#include "CheatOption.h"

class Cheat {
	vector <CheatOption*> options;
	map<LPCWSTR, bool> m_optionsState;
	LPCWSTR _processName = NULL;
	int processId = NULL;

	static DWORD WINAPI ProcessorStarter(void* param) {
		Cheat* that = reinterpret_cast<Cheat*>(param);
		that->ProcessOptions();
		return 0;
	}

	bool isRunning = false;

	void ProcessOptions();

public:

	Cheat(LPCWSTR processName) : _processName(processName) {}

	void Start() {
		if (!isRunning) {
			isRunning = true;
			DWORD threadId;
			CreateThread(NULL, 0, ProcessorStarter, this, NULL, &threadId);
		}
	}
	void Stop() { 
		isRunning = false; 
		for (CheatOption* op : options) {
			if (op->isEnabled() == true)
				op->Disable(processId);
			op->badDisable();
		}
	}

	std::map<LPCWSTR, bool>& GetCheatOptionsState() { return m_optionsState; }
	LPCWSTR GetProcessName() { return _processName; }
	bool isProcessRunning() { return processId; }

	int AddCheatOption(CheatOption* option);
	void RemoveCheatOption(int index);
};