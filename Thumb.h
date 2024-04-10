#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"

enum Axis { X, Y };

class ConditionPointVector;

class Thumb : public juce::Component
{
public:
    Thumb();
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void paint(juce::Graphics& g) override;
    void resized() override;
protected:
    juce::ComponentDragger mDragger;
    juce::ComponentBoundsConstrainer mConstrainer;
};
