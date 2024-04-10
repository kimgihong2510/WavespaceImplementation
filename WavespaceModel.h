#pragma once

#include <onnxruntime_cxx_api.h>
#include "GlobalVariables.h"
#include "Utils.h"
#include "model.ort.h"
#include <JuceHeader.h>
#include <cString>

class WavespaceModel
{
public:
    WavespaceModel();
    void setNewWaveform(ConditionData conditionData, std::array<float, SIZE_WAVEFORM>& waveform);

    std::array<float, SIZE_WAVEFORM> mWaveform;
    std::array<float, NUM_STYLE * 2 + NUM_DESCRIPTOR> mInputArray;
private:
    Ort::Env mEnv;
    std::unique_ptr<Ort::Session> mSession=nullptr;

    Ort::Value mInputTensor{nullptr};
    std::array<int64_t, 2> mInputShape;

    std::array<float, SIZE_AMP + SIZE_PHASE> mOutputArray;

    juce::dsp::FFT mFFT{SIZE_WAVEFORM_LOG2};
};