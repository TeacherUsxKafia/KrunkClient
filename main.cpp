#include <windows.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

HWND hWnd;
static ComPtr<ICoreWebView2> webviewWindow;

// THE PICO++ SECRET: Low-Level Mouse Hook
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && webviewWindow) {
        if (wParam == WM_LBUTTONDOWN) {
            webviewWindow->ExecuteScript(L"window.dispatchEvent(new MouseEvent('mousedown', {bubbles: true}));", nullptr);
            return 1; // Swallows the click to prevent browser delay
        }
        if (wParam == WM_LBUTTONUP) {
            webviewWindow->ExecuteScript(L"window.dispatchEvent(new MouseEvent('mouseup', {bubbles: true}));", nullptr);
            return 1;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    // Basic Window Setup
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, DefWindowProc, 0, 0, hInst, NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, L"KrunkerClass", NULL };
    RegisterClassEx(&wc);
    hWnd = CreateWindow(L"KrunkerClass", L"Open Krunker Native", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInst, NULL);
    ShowWindow(hWnd, nCmdShow);

    // Initialize WebView2
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [](HRESULT res, ICoreWebView2Controller* controller) -> HRESULT {
                        if (controller) {
                            controller->get_CoreWebView2(&webviewWindow);
                            webviewWindow->Navigate(L"https://krunker.io");
                            // Install the hook after webview is ready
                            SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(NULL), 0);
                        }
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
