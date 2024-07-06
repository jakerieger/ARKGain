// Author: Jake Rieger
// Created: 7/3/2024.
//

#include "View.h"

#define PLATFORM_WIN32 std::strcmp(type, kPlatformTypeHWND) == 0
#define PLATFORM_X11 std::strcmp(type, kPlatformTypeX11EmbedWindowID) == 0
#define PLATFORM_DARWIN std::strcmp(type, kPlatformTypeNSView) == 0

namespace ARK {
    using namespace Steinberg;

    tresult View::isPlatformTypeSupported(FIDString type) {
        if (PLATFORM_WIN32) {
            return kResultTrue;
        }

        return kResultFalse;
    }

    tresult View::attached(void* parent, FIDString type) {
        if (PLATFORM_WIN32) {
            // Create GUI here and attach it to the parent
            // For example, create a window and set its parent to the provided handle

            HWND parentHwnd = (HWND)parent;

            // Define a class for the window
            WNDCLASS wc      = {};
            wc.lpfnWndProc   = WindowProc;
            wc.hInstance     = GetModuleHandle(nullptr);
            wc.lpszClassName = L"ARKGainWindowClass";
            RegisterClass(&wc);

            // Create the window
            mHwnd = CreateWindowEx(0,                      // Optional window styles.
                                   L"ARKGainWindowClass",  // Window class
                                   L"ARKGain",             // Window text
                                   WS_CHILD | WS_VISIBLE,  // Window style
                                   0,
                                   0,
                                   600,
                                   300,                       // Size and position
                                   parentHwnd,                // Parent window
                                   nullptr,                   // Menu
                                   GetModuleHandle(nullptr),  // Instance handle
                                   this                       // Additional application data
            );

            if (!mHwnd)
                return kResultFalse;

            return kResultTrue;
        }

        return kResultFalse;
    }

    tresult View::removed() {
        if (mHwnd) {
            DestroyWindow(mHwnd);
            mHwnd = nullptr;
        }

        return kResultTrue;
    }

    tresult View::onWheel(float distance) {
        return EditorView::onWheel(distance);
    }

    tresult View::onKeyDown(char16 key, int16 keyCode, int16 modifiers) {
        return EditorView::onKeyDown(key, keyCode, modifiers);
    }

    tresult View::onKeyUp(char16 key, int16 keyCode, int16 modifiers) {
        return EditorView::onKeyUp(key, keyCode, modifiers);
    }

    tresult View::getSize(ViewRect* size) {
        size->left   = 0;
        size->top    = 0;
        size->right  = 600;  // Example width
        size->bottom = 300;  // Example height

        return kResultTrue;
    }

    tresult View::onSize(ViewRect* newSize) {
        if (mHwnd) {
            SetWindowPos(mHwnd,
                         nullptr,
                         0,
                         0,
                         newSize->getWidth(),
                         newSize->getHeight(),
                         SWP_NOZORDER | SWP_NOMOVE);
        }

        return kResultTrue;
    }

    tresult View::onFocus(TBool state) {
        return EditorView::onFocus(state);
    }

    tresult View::setFrame(IPlugFrame* frame) {
        return EditorView::setFrame(frame);
    }

    tresult View::canResize() {
        return EditorView::canResize();
    }

    tresult View::checkSizeConstraint(ViewRect* rect) {
        return EditorView::checkSizeConstraint(rect);
    }

    void View::Draw() const {}

    LRESULT CALLBACK View::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        View* view = nullptr;
        if (uMsg == WM_NCCREATE) {
            const auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            view           = static_cast<View*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(view));
        } else {
            view = reinterpret_cast<View*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (view) {
            switch (uMsg) {
                case WM_PAINT: {
                    view->Draw();
                    return 0;
                }

                case WM_DESTROY: {
                    PostQuitMessage(0);
                    return 0;
                }

                    // Handle other messages here...

                default:
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}  // namespace ARK