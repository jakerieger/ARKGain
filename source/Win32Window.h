// Author: Jake Rieger
// Created: 7/9/2024.
//

#pragma once

#include "Platform.h"

/// The Win32 API does some wacky stuff when you try to wrap it up in a non-static class.
/// So I've wrapped window logic in a namespace.
/// There is no cross-platform method of managing windows due to the way the VST3 SDK manages
/// the parent window. See `Steinberg::IPlugView` for more information.
namespace ARK::Win32Window {
    bool Create(HWND parent, LPCWSTR className, LPCWSTR title);
    void Destroy();

    HWND GetWindowHandle();

    static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}  // namespace ARK::Win32Window