/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TextComponent.h"
#include "ConditionPad.h"
#include "ShowWaveform.h"

#define SLIDER_H juce::Slider{juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox}
#define SLIDER_V juce::Slider{juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox}
//==============================================================================
/**
*/
class WavespaceAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    WavespaceAudioProcessorEditor (WavespaceAudioProcessor&);
    ~WavespaceAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *slider) override;

private:
    WavespaceAudioProcessor& mAudioProcessor;
    void setValues();

    ConditionPad mPadStyle;
    ConditionPad mPadDescriptor;
    ShowWaveform mPadWaveform;

    juce::Slider mSliderStyleType = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    juce::Slider mSliderDescriptorType = juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox);
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderStyleType;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderDescriptorType;
    
    juce::Slider mSliderStyleOnScreenX = juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox);
    juce::Slider mSliderStyleOnScreenY = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    juce::Slider mSliderDescriptorOnScreenX = juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox);
        
    juce::Slider mSliderGain = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxRight);
    juce::Slider mSliderAttack = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxRight);
    juce::Slider mSliderDecay = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxRight);
    juce::Slider mSliderSustain = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxRight);
    juce::Slider mSliderRelease = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxRight);
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentGain;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentAttack;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentDecay;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSustain;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentRelease;
    TextComponent mGainTextBox;
    TextComponent mAttackTextBox;
    TextComponent mDecayTextBox;
    TextComponent mSustainTextBox;
    TextComponent mReleaseTextBox;

    TextComponent mStyleTextBox;
    TextComponent mDescriptorTextBox;

    //NEED TO CHANGE
    std::array<juce::Slider, NUM_STYLE> mSliderStyleX={SLIDER_H, SLIDER_H};
    std::array<juce::Slider, NUM_STYLE> mSliderStyleY={SLIDER_V, SLIDER_V};
    std::array<juce::Slider, NUM_DESCRIPTOR> mSliderDescriptorX={SLIDER_H, SLIDER_H, SLIDER_H, SLIDER_H, SLIDER_H};

    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderStyleX0;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderStyleX1;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderStyleY0;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderStyleY1;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderDescriptorX0;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderDescriptorX1;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderDescriptorX2;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderDescriptorX3;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderDescriptorX4;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavespaceAudioProcessorEditor)
};
