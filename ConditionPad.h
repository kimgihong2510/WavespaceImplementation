#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"
#include "ConditionPoint.h"
#include "PluginProcessor.h"
#include <algorithm>

class ConditionPad : public juce::Component
{
public:
    ConditionPad(PadData padData);

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    enum Axis {X, Y};
    void sliderValueChanged(juce::String type, float value);
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
protected:
    ConditionPoint mConditionPoint{};

    PadData mPadData;    
};

