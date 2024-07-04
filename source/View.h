// Author: Jake Rieger
// Created: 7/3/2024.
//

#pragma once

#include "pluginterfaces/gui/iplugview.h"

// TODO: Implement IPlugView here. This is the class that handles the actual UI.

class View : public Steinberg::IPlugView {
public:
    View()          = default;
    virtual ~View() = default;
};
