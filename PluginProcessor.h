/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavespaceModel.h"
#include "Utils.h"
#include "Synthesizer.h"

//==============================================================================
/**
*/
class WavespaceAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    WavespaceAudioProcessor();
    ~WavespaceAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    Synthesizer mSynth{};
    CurConditionType mCurConditionType = {0, 0};
    ConditionData mConditionData = {{0.0f}, {0.0f}, {0.0f}};
private:
    void putApvtsToConditionData();
    WavespaceModel mWavespaceModel{};
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    float mSampleRate;
    int sampleCounter = 0;
    bool flagProcessBlockCalled = true;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavespaceAudioProcessor)
};
