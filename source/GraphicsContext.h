// Author: Jake Rieger
// Created: 7/10/2024.
//

#pragma once

namespace ARK {

    class GraphicsContext {
    public:
        GraphicsContext()  = default;
        ~GraphicsContext() = default;

        bool Initialize() {}
        void Shutdown() {}

        void Draw() {}

    private:
        void* m_Context;
    };

}  // namespace ARK
