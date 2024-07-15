// Author: Jake Rieger
// Created: 7/15/2024.
//

#pragma once

#include "Interfaces/PluginCanvas.h"

namespace ARK {

    class Canvas final : public ArkVector::IPluginCanvas {
    public:
        ArkVector::IComponent* Draw() override;
    };

}  // namespace ARK
