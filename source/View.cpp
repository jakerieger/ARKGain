// Author: Jake Rieger
// Created: 7/3/2024.
//

#include "View.h"
#include "Canvas.h"
#include "base/source/fstring.h"

#define PLATFORM_WIN32(X) std::strcmp((X), kPlatformTypeHWND) == 0
#define PLATFORM_X11(X) std::strcmp((X), kPlatformTypeX11EmbedWindowID) == 0
#define PLATFORM_DARWIN(X) std::strcmp((X), kPlatformTypeNSView) == 0

namespace ARK {
    using namespace Steinberg;

    tresult View::isPlatformTypeSupported(FIDString type) {
        m_Type = type;

        if (PLATFORM_WIN32(type) || PLATFORM_X11(type) || PLATFORM_DARWIN(type)) {
            return kResultTrue;
        }
        return kResultFalse;
    }

    tresult View::attached(void* parent, FIDString type) {
        m_Type = type;

        const tresult result = EditorView::attached(parent, type);
        if (result != kResultOk) {
            return result;
        }

        m_PluginCanvas = new Canvas();
        m_PluginCanvas->AttachToParent(parent);

        return kResultTrue;
    }

    tresult View::removed() {
        delete m_PluginCanvas;
        m_PluginCanvas = nullptr;

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
        if (newSize) {
            // update viewport
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
}  // namespace ARK