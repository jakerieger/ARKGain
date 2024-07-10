// Author: Jake Rieger
// Created: 7/9/2024.
//

#include "Win32Window.h"

#include "GraphicsContext.h"

namespace ARK::Win32Window {
    HWND g_Hwnd;
    std::unique_ptr<GraphicsContext> g_GraphicsContext;
    LPCWSTR g_ClassName;
    LPCWSTR g_WindowTitle;

    bool Create(HWND parent, LPCWSTR className, LPCWSTR title) {
        g_ClassName   = className;
        g_WindowTitle = title;

        if (!g_Hwnd) {
            WNDCLASSW wc     = {};
            wc.lpfnWndProc   = WindowProc;
            wc.hInstance     = GetModuleHandle(nullptr);
            wc.lpszClassName = className;

            const auto result = RegisterClassW(&wc);
            if (!result) {
                return false;
            }

            g_Hwnd = CreateWindowExW(0,
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

            if (!g_Hwnd) {
                return false;
            }
        }

        if (!g_GraphicsContext) {
            // Create graphics context
            g_GraphicsContext = std::make_unique<GraphicsContext>();
            if (!g_GraphicsContext->Initialize(&g_Hwnd)) {
                return false;
            }
        }

        return true;
    }

    void Destroy() {
        if (g_Hwnd) {
            DestroyWindow(g_Hwnd);
            g_Hwnd = nullptr;
            UnregisterClassW(g_ClassName, GetModuleHandle(nullptr));
        }

        if (g_GraphicsContext) {
            g_GraphicsContext->Shutdown(&g_Hwnd);
            g_GraphicsContext.reset();
        }
    }

    HWND GetWindowHandle() {
        return g_Hwnd;
    }

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc        = BeginPaint(hwnd, &ps);
                HBRUSH bgBrush = CreateSolidBrush(RGB(255, 11, 16));
                FillRect(hdc, &ps.rcPaint, bgBrush);
                DeleteObject(bgBrush);
                // Add any drawing code here...
                g_GraphicsContext->Draw();
                EndPaint(hwnd, &ps);
            } break;
            case WM_DESTROY: {
                g_Hwnd = nullptr;
            } break;
            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }
}  // namespace ARK::Win32Window