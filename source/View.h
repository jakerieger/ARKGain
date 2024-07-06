// Author: Jake Rieger
// Created: 7/3/2024.
//

#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

#include <Windows.h>
#include <cairo/cairo.h>

// TODO: Implement IPlugView here. This is the class that handles the actual UI.

namespace ARK {
    class View final : public Steinberg::Vst::EditorView {
    public:
        explicit View(Steinberg::Vst::EditController* controller)
            : EditorView(controller), mHwnd(nullptr) {}

        ~View() SMTG_OVERRIDE {
            DestroyCairoSurface();
        }

        Steinberg::tresult PLUGIN_API isPlatformTypeSupported(Steinberg::FIDString type)
          SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API attached(void* parent,
                                               Steinberg::FIDString type) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API removed() SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API onWheel(float distance) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API onKeyDown(Steinberg::char16 key,
                                                Steinberg::int16 keyCode,
                                                Steinberg::int16 modifiers) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API onKeyUp(Steinberg::char16 key,
                                              Steinberg::int16 keyCode,
                                              Steinberg::int16 modifiers) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API getSize(Steinberg::ViewRect* size) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API onSize(Steinberg::ViewRect* newSize) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API onFocus(Steinberg::TBool state) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API setFrame(Steinberg::IPlugFrame* frame) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API canResize() SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API checkSizeConstraint(Steinberg::ViewRect* rect) SMTG_OVERRIDE;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        void Draw() const;
        void CreateCairoSurface();
        void DestroyCairoSurface();

        HWND mHwnd;
        cairo_surface_t* mCairoSurface;
        cairo_t* mCairo;
    };
}  // namespace ARK
