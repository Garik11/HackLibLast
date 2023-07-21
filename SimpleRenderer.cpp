#include "SimpleRenderer.h"
LRESULT(*SimpleRenderer::baseProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT SimpleRenderer::ThisWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SimpleRenderer* pThis = reinterpret_cast<SimpleRenderer *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (pThis) pThis->DynamicWindowProc(hWnd, uMsg, wParam, lParam);

    return baseProc(hWnd, uMsg, wParam, lParam);
}

LRESULT SimpleRenderer::DynamicWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
        break;
    case WM_TIMER:
        if (wParam == 1000) this->RenderFrame();
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        BitBlt(hdc, _windowRect.left, _windowRect.top, _windowRect.right, _windowRect.bottom, _memDC, 0, 0, SRCCOPY);
        EndPaint(hWnd,&ps);
        break;
    default:
        break;
    }
    return 0;
}

void SimpleRenderer::RenderFrame()
{
    ZeroMemory(_memBitmapBits, _memDibSectionSize);

    HGDIOBJ oldFont = SelectObject(_memDC, _rstate.optionsFont);

    SetTextColor(_memDC, RGB(255, 255, 255));
    int deltaY = 25;
    for (auto& pair : _cheat->GetCheatOptionsState()) {
        if (pair.second == true) SetTextColor(_memDC, _rstate.enabledOptionsColor);
        else SetTextColor(_memDC, _rstate.optionsColor);
        TextOut(_memDC, 25, deltaY, pair.first, wcslen(pair.first));
        deltaY += 25;
    }
    SelectObject(_memDC, _rstate.processInfoFont);
    processInfo.clear();
    processInfo.append(_cheat->GetProcessName());
    processInfo.append(L" "); //stringstream

    if (_cheat->isProcessRunning()) {
        SetTextColor(_memDC, _rstate.processRunningColor);
        processInfo.append(L" is running");
        TextOut(_memDC, 25, _windowRect.bottom - 35, processInfo.c_str(), processInfo.length());
    }
    else {
        SetTextColor(_memDC, _rstate.processInfoColor);
        processInfo.append(L" is not running");
        TextOut(_memDC, 25, _windowRect.bottom - 35, processInfo.c_str(), processInfo.length()); //Нет смылса
    }

    SelectObject(_memDC, oldFont);
    RedrawWindow(_wnd, NULL, NULL, RDW_INVALIDATE);
}

HFONT SimpleRenderer::SimpleCreateFont(LPCWSTR fontFamily, int fontHeight, bool isItalic = false, bool isUnderline = false, bool isStrikedOut = false)
{
    return CreateFont(
        fontHeight, 0, 0, 0, 0, isItalic, isUnderline, isStrikedOut, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, fontFamily
    );
}

SimpleRenderer::SimpleRenderer(Cheat *cheat, LPCWSTR title, int width, int height): IRender(cheat, title, width, height)
{
    GetClientRect(_wnd, &_windowRect);

    BITMAPINFO bi;
    ZeroMemory(&bi, sizeof(BITMAPINFO));

    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = _windowRect.right;
    bi.bmiHeader.biHeight = _windowRect.bottom;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;

    _windowDC = GetDC(_wnd);
    _memDC = CreateCompatibleDC(_windowDC);
    //_memBitmap = CreateCompatibleBitmap(_windowDC, _windowRect.right, _windowRect.bottom);
    _memBitmap = CreateDIBSection(_memDC, &bi, DIB_RGB_COLORS, reinterpret_cast<LPVOID*>(&_memBitmapBits), NULL, NULL);
    _defMemBmp = SelectObject(_memDC, _memBitmap);
    _memDibSectionSize = _windowRect.right * _windowRect.bottom * 4L;

    SetWindowLongPtr(_wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    baseProc = (LRESULT(*)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam))reinterpret_cast<WNDPROC>(GetWindowLongPtr(_wnd, GWLP_WNDPROC)); //Ошибка
    SetWindowLongPtr(_wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ThisWindowProc));
}

void SimpleRenderer::Start()
{
    SetBkMode(_memDC, TRANSPARENT);
    _rstate.optionsFont = SimpleCreateFont(L"Arial", 20);
    _rstate.processInfoFont = SimpleCreateFont(L"Arial", 20);
    _rstate.optionsColor = RGB(255, 255, 255);
    _rstate.enabledOptionsColor = RGB(255, 0, 0);
    _rstate.processInfoColor = RGB(255, 0, 0);
    _rstate.processRunningColor = RGB(0, 128, 0);

    SetTimer(_wnd, 1000, 1000/60, NULL); // Требуется рефакторинг
    IRender::Start();
}

void SimpleRenderer::Stop()
{
    KillTimer(_wnd, 1000);
    SelectObject(_memDC, _defMemBmp);
    DeleteObject(_memBitmap);
    DeleteDC(_memDC);
    ReleaseDC(_wnd, _windowDC); 
}
