#pragma once
#include "IRender.h"

struct RENDER_STATE {
	HFONT optionsFont;
	HFONT processInfoFont;
	COLORREF optionsColor;
	COLORREF enabledOptionsColor;
	COLORREF processInfoColor;
	COLORREF processRunningColor;
};

class SimpleRenderer : public IRender {
private:
	HDC _windowDC = NULL;
	HDC _memDC = NULL;
	RECT _windowRect;
	HBITMAP _memBitmap = NULL;
	HGDIOBJ _defMemBmp = NULL; 
	BYTE* _memBitmapBits = NULL;
	LONG _memDibSectionSize = NULL;
	RENDER_STATE _rstate = { NULL };
	std::wstring processInfo;
	
	static LRESULT(*baseProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT ThisWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT DynamicWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RenderFrame();
	HFONT SimpleCreateFont(LPCWSTR fontFamily, int fontHeight, bool isItalic, bool isUnderline, bool isStrikedOut);
public:
	SimpleRenderer(Cheat* cheat, LPCWSTR title, int width, int height);

	void Start();
	void Stop();
};