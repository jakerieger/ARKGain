// Author: Jake Rieger
// Created: 7/9/2024.
//

#include "Win32Window.h"

namespace ARK::Win32Window {
    HWND k_Hwnd;

    bool Create(HWND parent, LPCWSTR className, LPCWSTR title) {
        if (!k_Hwnd) {
            WNDCLASSW wc     = {};
            wc.lpfnWndProc   = WindowProc;
            wc.hInstance     = GetModuleHandle(nullptr);
            wc.lpszClassName = className;
            auto result      = RegisterClassW(&wc);
            if (!result) {
                return false;
            }

            k_Hwnd = CreateWindowExW(0,
                                     className,
                                     title,
                                     WS_CHILD | WS_VISIBLE,  // Window style
                                     0,
                                     0,
                                     600,
                                     300,                       // Size and position
                                     parent,                    // Parent window
                                     nullptr,                   // Menu
                                     GetModuleHandle(nullptr),  // Instance handle
                                     nullptr);

            if (!k_Hwnd) {
                return false;
            }
        }

        return true;
    }

    void Destroy() {
        if (k_Hwnd) {
            DestroyWindow(k_Hwnd);
            k_Hwnd = nullptr;
        }
    }

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc        = BeginPaint(hwnd, &ps);
                HBRUSH bgBrush = CreateSolidBrush(RGB(10, 11, 16));
                FillRect(hdc, &ps.rcPaint, bgBrush);
                DeleteObject(bgBrush);
                // Add any drawing code here...
                EndPaint(hwnd, &ps);
            } break;
            case WM_DESTROY: {
                DestroyWindow(hwnd);
            } break;
            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }
}  // namespace ARK::Win32Window