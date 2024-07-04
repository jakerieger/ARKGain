//------------------------------------------------------------------------
// Copyright(c) 2024 ARKSYN Audio.
//------------------------------------------------------------------------

#include "Processor.h"
#include "Controller.h"
#include "CIDs.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "ARKGain"

using namespace Steinberg::Vst;
using namespace ARK;

//------------------------------------------------------------------------
//  VST Plug-in Entry
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF("ARKSYN Audio",
                  "https://github.com/jakerieger",
                  "mailto:contact.jakerieger@gmail.com")

//---First Plug-in included in this factory-------
// its kVstAudioEffectClass component
DEF_CLASS2(
  INLINE_UID_FROM_FUID(kARKGainProcessorUID),
  PClassInfo::kManyInstances,  // cardinality
  kVstAudioEffectClass,        // the component category (do not changed this)
  stringPluginName,            // here the Plug-in name (to be changed)
  Vst::kDistributable,  // means that component and controller could be distributed on different
                        // computers
  ARKGainVST3Category,  // Subcategory for this Plug-in (to be changed)
  FULL_VERSION_STR,     // Plug-in version (to be changed)
  kVstVersionString,    // the VST 3 SDK version (do not changed this, use always this define)
  ARKGainProcessor::createInstance)  // function pointer called when this component should be
                                     // instantiated

// its kVstComponentControllerClass component
DEF_CLASS2(
  INLINE_UID_FROM_FUID(kARKGainControllerUID),
  PClassInfo::kManyInstances,     // cardinality
  kVstComponentControllerClass,   // the Controller category (do not changed this)
  stringPluginName "Controller",  // controller name (could be the same than component name)
  0,                              // not used here
  "",                             // not used here
  FULL_VERSION_STR,               // Plug-in version (to be changed)
  kVstVersionString,  // the VST 3 SDK version (do not changed this, use always this define)
  ARKGainController::createInstance)  // function pointer called when this component should be
                                      // instantiated

//----for others Plug-ins contained in this factory, put like for the first Plug-in different
// DEF_CLASS2---

END_FACTORY
