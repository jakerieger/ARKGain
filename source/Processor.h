//------------------------------------------------------------------------
// Copyright(c) 2024 ARKSYN Audio.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace ARK {
    //------------------------------------------------------------------------
    //  ARKGainProcessor
    //------------------------------------------------------------------------
    class ARKGainProcessor : public Steinberg::Vst::AudioEffect {
    public:
        ARKGainProcessor();
        ~ARKGainProcessor() SMTG_OVERRIDE;

        // Create function
        static FUnknown* createInstance(void* /*context*/) {
            return static_cast<IAudioProcessor*>(new ARKGainProcessor);
        }

        //--- ---------------------------------------------------------------------
        // AudioEffect overrides:
        //--- ---------------------------------------------------------------------
        /** Called at first after constructor */
        Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) SMTG_OVERRIDE;

        /** Called at the end before destructor */
        Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

        /** Switch the Plug-in on/off */
        Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) SMTG_OVERRIDE;

        /** Will be called before any process call */
        Steinberg::tresult PLUGIN_API setupProcessing(Steinberg::Vst::ProcessSetup& newSetup)
          SMTG_OVERRIDE;

        /** Asks if a given sample size is supported see SymbolicSampleSizes. */
        Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize)
          SMTG_OVERRIDE;

        /** Here we go...the process call */
        Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;

        /** For persistence */
        Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) SMTG_OVERRIDE;
        Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) SMTG_OVERRIDE;

        template<typename SampleType>
        Steinberg::tresult PLUGIN_API processAudio(Steinberg::Vst::ProcessData& data) const;

        //------------------------------------------------------------------------
    protected:
        float m_Gain;
        float m_Balance;
    };

    //------------------------------------------------------------------------
}  // namespace ARK
