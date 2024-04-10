#pragma once

#include <JuceHeader.h>
#include "GlobalVariables.h"
#include "WavespaceModel.h"
#include "Utils.h"

class Synthesizer
{
public:
    void setFrequencyZero(float frequency, float sampleRate);
    float getNextSample();
    void removeKeyOn(float frequency, float sampleRate);

    float mAttackDuration = 0.1f;
    float mReleaseDuration = 0.1f;
    float mDecayDuration = 0.1f;
    float mSustain = 1.0f;
    float mGain = 1.0f;
    std::array<float, SIZE_WAVEFORM> mWaveform;
 
private:
    void addKeyOn(float frequency);
    void updateAngle();

    float currentAngle[NUM_SYNTH_POLYPHONY] = {0};
    float angleDelta[NUM_SYNTH_POLYPHONY] = {0};
    bool keyboardOn[NUM_SYNTH_POLYPHONY] = {false};
    float mNumKeyOnSample[NUM_SYNTH_POLYPHONY];

    float mInRelease[NUM_SYNTH_POLYPHONY]={0};
    float mInAttack[NUM_SYNTH_POLYPHONY]={0};
    float mInDecay[NUM_SYNTH_POLYPHONY]={0}; 
    float mBeforeReleaseAmp[NUM_SYNTH_POLYPHONY]={0}; 

    float mSampleRate;
};
