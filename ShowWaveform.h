#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"

//==============================================================================
/**
*/

class ShowWaveform : public juce::Component
{
public:
    ShowWaveform(float* ptr);
    void paint (juce::Graphics& g) override;
    void resized() override;
private:
    float* mPtrWaveform;
};
