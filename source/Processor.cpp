//------------------------------------------------------------------------
// Copyright(c) 2024 ARKSYN Audio.
//------------------------------------------------------------------------

#include "Processor.h"
#include "CIDs.h"
#include "ParameterIds.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace ARK {
    namespace DSP {
        float dBToLinear(const float db) {
            return std::pow(10.f, db / 20.f);
        }
    }  // namespace DSP

    namespace Processors {
        template<typename SampleType>
        tresult MonoToMono(SampleType** inputBuffers,
                           SampleType** outputBuffers,
                           const int32 numFrames,
                           float gain) {
            SampleType* inputMono  = inputBuffers[0];
            SampleType* outputMono = outputBuffers[0];

            for (int32 n = 0; n < numFrames; n++) {
                outputMono[n] = inputMono[n] * gain;
            }

            return kResultOk;
        }

        template<typename SampleType>
        tresult StereoToMono(SampleType** inputBuffers,
                             SampleType** outputBuffers,
                             const int32 numFrames,
                             float gain) {
            return MonoToMono(inputBuffers, outputBuffers, numFrames, gain);
        }

        template<typename SampleType>
        tresult MonoToStereo(SampleType** inputBuffers,
                             SampleType** outputBuffers,
                             const int32 numFrames,
                             float gain) {
            SampleType* inputMono   = inputBuffers[0];
            SampleType* outputLeft  = outputBuffers[0];
            SampleType* outputRight = outputBuffers[1];

            for (int32 n = 0; n < numFrames; n++) {
                outputLeft[n]  = inputMono[n] * gain;
                outputRight[n] = inputMono[n] * gain;
            }

            return kResultOk;
        }

        template<typename SampleType>
        tresult StereoToStereo(SampleType** inputBuffers,
                               SampleType** outputBuffers,
                               const int32 numFrames,
                               float gain) {
            SampleType* inputLeft   = inputBuffers[0];
            SampleType* inputRight  = inputBuffers[1];
            SampleType* outputLeft  = outputBuffers[0];
            SampleType* outputRight = outputBuffers[1];

            for (int32 n = 0; n < numFrames; n++) {
                outputLeft[n]  = inputLeft[n] * gain;
                outputRight[n] = inputRight[n] * gain;
            }

            return kResultOk;
        }
    }  // namespace Processors

    //------------------------------------------------------------------------
    // ARKGainProcessor
    //------------------------------------------------------------------------
    ARKGainProcessor::ARKGainProcessor() {
        //--- set the wanted controller for our processor
        setControllerClass(kARKGainControllerUID);
        m_Gain    = 0.0f;
        m_Balance = 0.0f;
    }

    //------------------------------------------------------------------------
    ARKGainProcessor::~ARKGainProcessor() = default;

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
        //--- called when the Plug-in is enabled/disabled (On/Off) -----
        return AudioEffect::setActive(state);
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::process(Vst::ProcessData& data) {
        //--- First : Read inputs parameter changes-----------
        if (data.inputParameterChanges) {
            int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
            for (int32 index = 0; index < numParamsChanged; index++) {
                if (auto* paramQueue = data.inputParameterChanges->getParameterData(index)) {
                    Vst::ParamValue value;
                    int32 sampleOffset;
                    int32 numPoints = paramQueue->getPointCount();
                    switch (paramQueue->getParameterId()) {
                        case kGain:
                            if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) ==
                                kResultTrue) {
                                m_Gain = static_cast<float>(value * 90.0 -
                                                            60.0);  // convert back from normalized
                                                                    // param value (-60.f <-> 30.f)
                            }
                            break;
                        case kBalance:
                            if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) ==
                                kResultTrue) {
                                m_Balance = static_cast<float>(
                                  value * 100.0 - 50.0);  // convert back from normalized
                                // param value (-50.f <-> 50.f)
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        // Do nothing if no data
        if (data.numInputs == 0 || data.numOutputs == 0 || data.numSamples == 0) {
            return kResultOk;
        }

        // Pass off to processing method
        return this->processAudio<float>(data);
    }

    template<typename SampleType>
    // ReSharper disable once CppDFAConstantFunctionResult
    tresult ARKGainProcessor::processAudio(Vst::ProcessData& data) const {
        using namespace Steinberg::Vst;

        const int32 numFrames         = data.numSamples;
        const uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, numFrames);
        auto** currentInputBuffers =
          reinterpret_cast<SampleType**>(getChannelBuffersPointer(processSetup, data.inputs[0]));
        auto** currentOutputBuffers =
          reinterpret_cast<SampleType**>(getChannelBuffersPointer(processSetup, data.outputs[0]));

        // If we have only silence, clear the output and do nothing
        data.outputs->silenceFlags = data.inputs->silenceFlags ? 0x7FFFF : 0;
        if (data.inputs->silenceFlags) {
            memset(currentOutputBuffers[0], 0, sampleFramesSize);

            if (data.outputs->numChannels > 1) {
                memset(currentOutputBuffers[1], 0, sampleFramesSize);
            }

            return kResultOk;
        }

        const auto numInChannels  = data.inputs->numChannels;
        const auto numOutChannels = data.outputs->numChannels;

        auto linearGain = DSP::dBToLinear(m_Gain);

        // Mono to Mono
        if (numInChannels == 1 && numOutChannels == 1) {
            return Processors::MonoToMono(currentInputBuffers,
                                          currentOutputBuffers,
                                          numFrames,
                                          linearGain);
        }
        // Mono to Stereo
        if (numInChannels == 1 && numOutChannels == 2) {
            return Processors::MonoToStereo(currentInputBuffers,
                                            currentOutputBuffers,
                                            numFrames,
                                            linearGain);
        }
        // Stereo to Mono
        if (numInChannels == 2 && numOutChannels == 1) {
            return Processors::StereoToMono(currentInputBuffers,
                                            currentOutputBuffers,
                                            numFrames,
                                            linearGain);
        }
        // Stereo to Stereo
        if (numInChannels == 2 && numOutChannels == 2) {
            return Processors::StereoToStereo(currentInputBuffers,
                                              currentOutputBuffers,
                                              numFrames,
                                              linearGain);
        }

        return kResultFalse;
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
        if (streamer.readFloat(m_Gain) == false) {
            return kResultFalse;
        }

        if (streamer.readFloat(m_Balance) == false) {
            return kResultFalse;
        }

        return kResultOk;
    }

    //------------------------------------------------------------------------
    tresult PLUGIN_API ARKGainProcessor::getState(IBStream* state) {
        // here we need to save the model
        IBStreamer streamer(state, kLittleEndian);
        streamer.writeFloat(m_Gain);
        streamer.writeFloat(m_Balance);

        return kResultOk;
    }

    //------------------------------------------------------------------------
}  // namespace ARK
