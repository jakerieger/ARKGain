//------------------------------------------------------------------------
// Copyright(c) 2024 ARKSYN Audio.
//------------------------------------------------------------------------

#include "Controller.h"
#include "CIDs.h"
#include "View.h"
#include "ParameterIds.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ustring.h"

using namespace Steinberg;

namespace ARK {

    //------------------------------------------------------------------------
    // ARKGainController Implementation
    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainController::initialize(FUnknown* context) {
        // Here the Plug-in will be instantiated

        //---do not forget to call parent ------
        tresult result = EditControllerEx1::initialize(context);
        if (result != kResultOk) {
            return result;
        }

        // Here you could register some parameters
        const auto gainParam =
          new Vst::RangeParameter(USTRING("Gain"), kGain, USTRING("dB"), -60.0f, 30.0f, 0.000001f);
        const auto balanceParam =
          new Vst::RangeParameter(USTRING("Balance"), kBalance, USTRING("R"), -50.0f, 50.0f, 0.0f);

        parameters.addParameter(gainParam);
        parameters.addParameter(balanceParam);

        return result;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainController::terminate() {
        // Here the Plug-in will be de-instantiated, last possibility to remove some memory!

        //---do not forget to call parent ------
        return EditControllerEx1::terminate();
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainController::setComponentState(IBStream* state) {
        // Here you get the state of the component (Processor part)
        if (!state)
            return kResultFalse;

        IBStreamer streamer(state, kLittleEndian);

        float gain;
        float balance;

        if (streamer.readFloat(gain) == false) {
            return kResultFalse;
        }

        if (streamer.readFloat(balance) == false) {
            return kResultFalse;
        }

        setParamNormalized(kGain, gain);
        setParamNormalized(kBalance, balance);

        return kResultOk;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainController::setState(IBStream* state) {
        // Here you get the state of the controller

        return kResultTrue;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainController::getState(IBStream* state) {
        // Here you are asked to deliver the state of the controller (if needed)
        // Note: the real state of your plug-in is saved in the processor

        return kResultTrue;
    }

    //------------------------------------------------------------------------
    IPlugView* PLUGIN_API ARKGainController::createView(FIDString name) {
        // Here the Host wants to open your editor (if you have one)
        if (FIDStringsEqual(name, Vst::ViewType::kEditor)) {
            // create your editor here and return a IPlugView ptr of it
            auto* view = new View(this);
            return view;
        }
        return nullptr;
    }

    //------------------------------------------------------------------------
}  // namespace ARK
