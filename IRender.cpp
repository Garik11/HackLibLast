#include "IRender.h"
#include "resource.h"
LRESULT IRender::BaseWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(S_OK);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

void IRender::CreateBaseWindow(LPCWSTR title, int w, int h) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wcx = { 0 };
	HWND mainWnd = NULL;

	wcx.cbSize = sizeof wcx;
	wcx.hInstance = hInstance;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.hbrBackground = NULL;
	wcx.hIcon =  static_cast<HICON>(::LoadImage(hInstance,
		MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON,
		256, 256,
		LR_DEFAULTCOLOR));

	wcx.lpfnWndProc = (WNDPROC)BaseWindowProc;
	wcx.lpszClassName = L"GameHackLabTrainer";

	RegisterClassEx(&wcx);

	_wnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		wcx.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - w) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - h) / 2,
		w,
		h,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(_wnd, SW_SHOWDEFAULT);
	UpdateWindow(_wnd);
	SetForegroundWindow(_wnd);
}

void IRender::StartMessageCycle() {
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

IRender::IRender(Cheat *cheat, LPCWSTR title, int width, int height): _cheat(cheat)
{
	CreateBaseWindow(title, width, height);
}
