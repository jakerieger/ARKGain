// Author: Jake Rieger
// Created: 7/15/2024.
//

#include "Canvas.h"
#include "Box.h"

namespace ARK {
    ArkVector::IComponent* Canvas::Draw() {
        return new ArkVector::Component::Box;
    }
}  // namespace ARK