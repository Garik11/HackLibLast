#include "Cheat.h"
#include "memory_functions.h"

void Cheat::ProcessOptions()
{
	while (isRunning) {
		processId = GetProcessIdByProcessName(_processName);
		if (processId) {
			for (CheatOption* option : options) {
				option->Process(processId);
				m_optionsState[option->GetDescription()] = option->isEnabled();
			}
		}
		else {
			for (CheatOption* option : options) {
				option->badDisable();
				m_optionsState[option->GetDescription()] = option->isEnabled();
			}
		}
		Sleep(50);
	}

}

int Cheat::AddCheatOption(CheatOption* option) {
	options.push_back(option);
	m_optionsState.insert(make_pair(option->GetDescription(), option->isEnabled()));
	return options.size() - 1;
}

void Cheat::RemoveCheatOption(int index) { 
	m_optionsState.erase(options[index]->GetDescription());
	options.erase(options.begin() + index); 
}
