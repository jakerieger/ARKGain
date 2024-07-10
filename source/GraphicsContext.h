// Author: Jake Rieger
// Created: 7/10/2024.
//

#pragma once

#include "Platform.h"

#include <cassert>
#include <nanosvg.h>
#include <gl/GL.h>
#include <gl/GLU.h>

namespace ARK {
    class IGraphicsContext {
    public:
        virtual ~IGraphicsContext() = default;
        virtual bool Initialize() {
            return true;
        }
        virtual void Shutdown() {}
        virtual void Draw() = 0;
    };

    class WindowsGraphicsContext final : public IGraphicsContext {
    public:
        WindowsGraphicsContext() : m_HDC(nullptr), m_HRC(nullptr) {}

        bool Initialize(const HWND* window);
        void Shutdown(const HWND* window);
        void Draw() override;

    private:
        [[nodiscard]] bool InitOpenGL() const;

        HDC m_HDC;
        HGLRC m_HRC;
    };

}  // namespace ARK
