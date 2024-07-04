//------------------------------------------------------------------------
// Copyright(c) 2024 ARKSYN Audio.
//------------------------------------------------------------------------

#include "ARKprocessor.h"
#include "ARKcids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"
#include "pluginterfaces/base/ibstream.h"
#include "public.sdk/source/vst/utility/audiobuffers.h"

using namespace Steinberg;

namespace ARK {
    //------------------------------------------------------------------------
    // ARKGainProcessor
    //------------------------------------------------------------------------
    ARKGainProcessor::ARKGainProcessor() {
        //--- set the wanted controller for our processor
        setControllerClass(kARKGainControllerUID);
    }

    //------------------------------------------------------------------------
    ARKGainProcessor::~ARKGainProcessor() {}

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::initialize(FUnknown* context) {
        // Here the Plug-in will be instantiated

        //---always initialize the parent-------
        tresult result = AudioEffect::initialize(context);
        // if everything Ok, continue
        if (result != kResultOk) {
            return result;
        }

        //--- create Audio IO ------
        addAudioInput(STR16("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
        addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

        /* If you don't need an event bus, you can remove the next line */
        addEventInput(STR16("Event In"), 1);

        return kResultOk;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::terminate() {
        // Here the Plug-in will be de-instantiated, last possibility to remove some memory!

        //---do not forget to call parent ------
        return AudioEffect::terminate();
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::setActive(TBool state) {
        //--- called when the Plug-in is enable/disable (On/Off) -----
        return AudioEffect::setActive(state);
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::process(Vst::ProcessData& data) {
        //--- First : Read inputs parameter changes-----------

        /*if (data.inputParameterChanges)
        {
            int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
            for (int32 index = 0; index < numParamsChanged; index++)
            {
                if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
                {
                    Vst::ParamValue value;
                    int32 sampleOffset;
                    int32 numPoints = paramQueue->getPointCount ();
                    switch (paramQueue->getParameterId ())
                    {
                    }
                }
            }
        }*/

        // //--- Here you have to implement your processing
        //
        // if (data.numSamples > 0) {
        //     //--- ------------------------------------------
        //     // here as example a default implementation where we try to copy the inputs to the
        //     // outputs: if less input than outputs then clear outputs
        //     //--- ------------------------------------------
        //
        //     int32 minBus = std::min(data.numInputs, data.numOutputs);
        //     for (int32 i = 0; i < minBus; i++) {
        //         int32 minChan = std::min(data.inputs[i].numChannels,
        //         data.outputs[i].numChannels); for (int32 c = 0; c < minChan; c++) {
        //             // do not need to be copied if the buffers are the same
        //             if (data.outputs[i].channelBuffers32[c] !=
        //             data.inputs[i].channelBuffers32[c]) {
        //                 memcpy(data.outputs[i].channelBuffers32[c],
        //                        data.inputs[i].channelBuffers32[c],
        //                        data.numSamples * sizeof(Vst::Sample32));
        //             }
        //         }
        //         data.outputs[i].silenceFlags = data.inputs[i].silenceFlags;
        //
        //         // clear the remaining output buffers
        //         for (int32 c = minChan; c < data.outputs[i].numChannels; c++) {
        //             // clear output buffers
        //             memset(data.outputs[i].channelBuffers32[c],
        //                    0,
        //                    data.numSamples * sizeof(Vst::Sample32));
        //
        //             // inform the host that this channel is silent
        //             data.outputs[i].silenceFlags |= ((uint64)1 << c);
        //         }
        //     }
        //     // clear the remaining output buffers
        //     for (int32 i = minBus; i < data.numOutputs; i++) {
        //         // clear output buffers
        //         for (int32 c = 0; c < data.outputs[i].numChannels; c++) {
        //             memset(data.outputs[i].channelBuffers32[c],
        //                    0,
        //                    data.numSamples * sizeof(Vst::Sample32));
        //         }
        //         // inform the host that this bus is silent
        //         data.outputs[i].silenceFlags = ((uint64)1 << data.outputs[i].numChannels) - 1;
        //     }
        // }

        if (data.numInputs == 0 || data.numOutputs == 0 || data.numSamples == 0) {
            return kResultOk;
        }

        return this->processAudio<float>(data);
    }

    template<typename SampleType>
    tresult ARKGainProcessor::processAudio(Steinberg::Vst::ProcessData& data) const {
        using namespace Steinberg::Vst;

        const int32 numFrames   = data.numSamples;
        uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, numFrames);
        auto** currentInputBuffers =
          (SampleType**)getChannelBuffersPointer(processSetup, data.inputs[0]);
        auto** currentOutputBuffers =
          (SampleType**)getChannelBuffersPointer(processSetup, data.outputs[0]);

        // If we have only silence, clear the output and do nothing
        data.outputs->silenceFlags = data.inputs->silenceFlags ? 0x7FFFF : 0;
        if (data.inputs->silenceFlags) {
            memset(currentOutputBuffers[0], 0, sampleFramesSize);

            if (data.numOutputs > 1) {
                memset(currentOutputBuffers[1], 0, sampleFramesSize);
            }

            return kResultOk;
        }

        SampleType tmp;
        SampleType* inputMono   = currentInputBuffers[0];
        SampleType* outputLeft  = currentOutputBuffers[0];
        SampleType* outputRight = currentOutputBuffers[1];

        constexpr float kGain = 20.f;  // TODO: Hard-coded test value

        for (int32 n = 0; n < numFrames; n++) {
            tmp           = inputMono[n];
            outputLeft[n] = tmp * kGain;

            if (data.numOutputs > 1) {
                outputRight[n] = tmp * kGain;
            }
        }

        return kResultOk;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::setupProcessing(Vst::ProcessSetup& newSetup) {
        //--- called before any processing ----
        return AudioEffect::setupProcessing(newSetup);
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
        // by default kSample32 is supported
        // disable the trailing comment if your processing supports kSample64
        if (symbolicSampleSize == Vst::kSample32 /*|| symbolicSampleSize == Vst::kSample64*/)
            return kResultTrue;

        return kResultFalse;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::setState(IBStream* state) {
        // called when we load a preset, the model has to be reloaded
        IBStreamer streamer(state, kLittleEndian);

        return kResultOk;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::getState(IBStream* state) {
        // here we need to save the model
        IBStreamer streamer(state, kLittleEndian);

        return kResultOk;
    }

    //------------------------------------------------------------------------
}  // namespace ARK
