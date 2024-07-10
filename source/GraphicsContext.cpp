// Author: Jake Rieger
// Created: 7/10/2024.
//

#include "GraphicsContext.h"

namespace ARK {
    bool WindowsGraphicsContext::Initialize(const HWND* window) {
        PIXELFORMATDESCRIPTOR pfd = {};
        m_HDC                     = GetDC(*window);

        ZeroMemory(&pfd, sizeof(pfd));
        pfd.nSize      = sizeof(pfd);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 16;
        pfd.iLayerType = PFD_MAIN_PLANE;

        const int format = ChoosePixelFormat(m_HDC, &pfd);
        SetPixelFormat(m_HDC, format, &pfd);

        m_HRC = wglCreateContext(m_HDC);
        wglMakeCurrent(m_HDC, m_HRC);

        return InitOpenGL();
    }

    void WindowsGraphicsContext::Shutdown(const HWND* window) {
        if (m_HRC) {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(m_HRC);
            m_HRC = nullptr;
        }

        if (m_HDC) {
            ReleaseDC(*window, m_HDC);
            m_HDC = nullptr;
        }
    }

    void WindowsGraphicsContext::Draw() {
        glClearColor(0.f, 1.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SwapBuffers(m_HDC);
    }

    bool WindowsGraphicsContext::InitOpenGL() const {
        assert(m_HDC);
        return true;
    }
}  // namespace ARK