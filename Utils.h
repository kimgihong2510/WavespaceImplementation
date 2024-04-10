#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"

juce::StringArray splitString(const juce::String& text, const juce::String& delimiter);
juce::String makeParamID(juce::String condType, juce::String pointType, int numCond);

void performIFFT(juce::dsp::FFT& fft, const float* ampArray, const float* phaseArray, std::array<float, SIZE_WAVEFORM*2>& fftArray);
inline std::complex<float> performPolar(float amp, float phase);

juce::Rectangle<int> createSquareFromRectangle(juce::Rectangle<int>* bounds);

bool isTwoFloatSame(float a, float b);

typedef struct curConditionType
{
    unsigned int curTypeStyle;
    unsigned int curTypeDescriptor;
}CurConditionType;

typedef struct conditionData
{
    std::array<float, NUM_STYLE> valuesStyleX;
    std::array<float, NUM_STYLE> valuesStyleY;
    std::array<float, NUM_DESCRIPTOR> valuesDescriptorX;
}ConditionData;

typedef struct padData
{
    int NUM_COND;
    juce::String COND_NAME;
    float COND_MIN;
    float COND_MAX;

    juce::Slider* sliderX;
    juce::Slider* sliderY;
    juce::Slider::Listener* parent_ptr;
}PadData;