//------------------------------------------------------------------------
// Copyright(c) 2024 ARKSYN Audio.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace ARK {
    //------------------------------------------------------------------------
    static const Steinberg::FUID
      kARKGainProcessorUID(0x92F2B27B, 0x99945A8B, 0xBD45AC45, 0xA00B357A);
    static const Steinberg::FUID
      kARKGainControllerUID(0xB7576D7A, 0xE01E5AEF, 0x9DAD67D4, 0x395BFB42);

#define ARKGainVST3Category "Fx"

    //------------------------------------------------------------------------
}  // namespace ARK
