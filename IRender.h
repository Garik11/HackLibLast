#pragma once

#include <Windows.h>
#include <map>

#include "Cheat.h"

class IRender {
public:
	HWND _wnd = NULL;
	Cheat* _cheat = NULL;

	static LRESULT BaseWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void CreateBaseWindow(LPCWSTR title, int w, int h);
	void StartMessageCycle();

public:
	IRender(Cheat *cheat, LPCWSTR title, int width, int height);
	virtual void Start() { StartMessageCycle(); }
	virtual void Stop() = 0;
};
