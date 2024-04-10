/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WavespaceModel.h"

//==============================================================================
WavespaceAudioProcessor::WavespaceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, juce::Identifier("apvts_WSS"), createParameterLayout())
#endif
{
    for (auto* param : this->getParameters())
    {
        if (auto* paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
        {
            auto paramID = paramWithID->getParameterID();
            apvts.addParameterListener(paramID, this);
        }
    }
    putApvtsToConditionData();
    mWavespaceModel.setNewWaveform(mConditionData, mSynth.mWaveform);
}

WavespaceAudioProcessor::~WavespaceAudioProcessor()
{
}

//==============================================================================
const juce::String WavespaceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WavespaceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WavespaceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WavespaceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WavespaceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WavespaceAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WavespaceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WavespaceAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WavespaceAudioProcessor::getProgramName (int index)
{
    return {};
}

void WavespaceAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WavespaceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampleRate = sampleRate;
}

void WavespaceAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WavespaceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WavespaceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    sampleCounter += buffer.getNumSamples();
    if(sampleCounter >= mSampleRate / LOAD_MODEL_PER_SEC)
        sampleCounter = mSampleRate / LOAD_MODEL_PER_SEC;
    flagProcessBlockCalled = true;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (const juce::MidiMessageMetadata midiMetaData : midiMessages)
    {
        auto midiMessage = midiMetaData.getMessage();

        if (midiMessage.isNoteOn())
        {
            auto freq = midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber(), 440.f);
            mSynth.setFrequencyZero(static_cast<float>(freq), mSampleRate);
        }
        else if (midiMessage.isNoteOff())
        {
            auto freq = midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber(), 440.f);
            mSynth.removeKeyOn(static_cast<float>(freq), mSampleRate);
        }
    }

    for(int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        auto nextSample = mSynth.getNextSample();

        for(int channel=0; channel<totalNumOutputChannels; channel++)
        {
            auto channelData = buffer.getWritePointer(channel);
            channelData[sample] = nextSample;
        }
    }
}

//==============================================================================
bool WavespaceAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WavespaceAudioProcessor::createEditor()
{
    return new WavespaceAudioProcessorEditor (*this);
}

//==============================================================================
void WavespaceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void WavespaceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
    }
    putApvtsToConditionData();
    mWavespaceModel.setNewWaveform(mConditionData, mSynth.mWaveform);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavespaceAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout WavespaceAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //gain, ADSR
    auto name = juce::String("gain");
    auto ID = juce::ParameterID{name, 1};
    auto range =  juce::NormalisableRange<float> (0.0f, 5.0f, 0.01f);
    auto defaultValue = 1.0f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    name = juce::String("attack");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (ADSR_MIN, ADSR_MAX, 0.01f);
    defaultValue = 0.1f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    name = juce::String("decay");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (ADSR_MIN, ADSR_MAX, 0.01f);
    defaultValue = 0.1f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    name = juce::String("sustain");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f);
    defaultValue = 1.0f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    name = juce::String("release");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (ADSR_MIN, ADSR_MAX, 0.01f);
    defaultValue = 0.1f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    name = juce::String("TYPE_STYLE");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (0, NUM_STYLE-1, 1.0f);
    defaultValue = 0.0f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    name = juce::String("TYPE_DESCRIPTOR");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (0, NUM_DESCRIPTOR-1, 1.0f);
    defaultValue = 0.0f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    for(int i = 0; i < NUM_STYLE; i++)
    {
        name = juce::String("STYLE_X_") + juce::String(i);
        ID = juce::ParameterID{name, 1};
        range =  juce::NormalisableRange<float> (STYLE_CONDITION_MIN, STYLE_CONDITION_MAX, 0.01f);
        defaultValue = 0.0f;
        layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

        name = juce::String("STYLE_Y_") + juce::String(i);
        ID = juce::ParameterID{name, 1};
        range =  juce::NormalisableRange<float> (STYLE_CONDITION_MIN, STYLE_CONDITION_MAX, 0.01f);
        defaultValue = 0.0f;
        layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));
    }
    for(int i = 0; i < NUM_DESCRIPTOR; i++)
    {
        name = juce::String("DESCRIPTOR_X_") + juce::String(i);
        ID = juce::ParameterID{name, 1};
        range =  juce::NormalisableRange<float> (DESCRIPTOR_CONDITION_MIN, DESCRIPTOR_CONDITION_MAX, 0.01f);
        defaultValue = 0.0f;
        if(i == IDX_SYMMETRY)
            defaultValue = 0.5f;
        layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));
    }

    return layout;
}

void WavespaceAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    auto splitStr = splitString(parameterID, "_");
    if(splitStr.size() == 1) //amp env params
    {
        auto paramName = splitStr[0];
        if(paramName.equalsIgnoreCase("gain")) {mSynth.mGain = newValue;}
        if(paramName.equalsIgnoreCase("attack")) {mSynth.mAttackDuration = newValue;}
        if(paramName.equalsIgnoreCase("decay")) {mSynth.mDecayDuration = newValue;}
        if(paramName.equalsIgnoreCase("sustain")) {mSynth.mSustain = newValue;}
        if(paramName.equalsIgnoreCase("release")) {mSynth.mReleaseDuration = newValue;}
    }
    else if(splitStr.size() == 2) // type num params
    {
        auto paramName = splitStr[1];
        if(paramName.equalsIgnoreCase("STYLE")) {mCurConditionType.curTypeStyle = static_cast<unsigned int>(newValue);}
        if(paramName.equalsIgnoreCase("DESCRIPTOR")) {mCurConditionType.curTypeDescriptor= static_cast<unsigned int>(newValue);}
    }
    else if(splitStr.size() == 3)// condition value params
    {
        auto condName = splitStr[0];
        auto condDim = splitStr[1];
        int condNum = splitStr[2].getIntValue();
        if(condName.equalsIgnoreCase("STYLE"))
        {
            if(condDim.equalsIgnoreCase("X"))
                mConditionData.valuesStyleX[condNum] = newValue;
            else
                mConditionData.valuesStyleY[condNum] = newValue;
        }
        else if(condName.equalsIgnoreCase("DESCRIPTOR"))
        {
            mConditionData.valuesDescriptorX[condNum] = newValue;
        }
        if(flagProcessBlockCalled && sampleCounter >= mSampleRate / LOAD_MODEL_PER_SEC)
        {
            flagProcessBlockCalled=false;
            sampleCounter = 0;
            mWavespaceModel.setNewWaveform(mConditionData, mSynth.mWaveform);
        }
    }
}

void WavespaceAudioProcessor::putApvtsToConditionData()
{
    for(int i = 0; i < NUM_STYLE; i++)
    {
        auto name = juce::String("STYLE_X_") + juce::String(i);
        mConditionData.valuesStyleX[i] = apvts.getRawParameterValue(name)->load();
        name = juce::String("STYLE_Y_") + juce::String(i);
        mConditionData.valuesStyleY[i] = apvts.getRawParameterValue(name)->load();
    }
    for(int i = 0; i < NUM_DESCRIPTOR; i++)
    {
        auto name = juce::String("DESCRIPTOR_X_") + juce::String(i);
        mConditionData.valuesDescriptorX[i] = apvts.getRawParameterValue(name)->load();
    }
    auto name = juce::String("TYPE_STYLE");
    mCurConditionType.curTypeStyle = apvts.getRawParameterValue(name)->load();
    name = juce::String("TYPE_DESCRIPTOR");
    mCurConditionType.curTypeDescriptor = apvts.getRawParameterValue(name)->load();
}