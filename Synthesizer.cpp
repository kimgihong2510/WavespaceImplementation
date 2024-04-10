#include "Synthesizer.h"

void Synthesizer::setFrequencyZero(float frequency, float sampleRate)
{
    auto cyclesPerSample = frequency / sampleRate;
    mSampleRate = sampleRate;
    addKeyOn(cyclesPerSample * PI * 2);
}

float Synthesizer::getNextSample()
{
    float sum = 0.0f;
    
    for(int i = 0; i < NUM_SYNTH_POLYPHONY; i++)
    {
        float amp = 0.0f;
        if(keyboardOn[i])
        {
            if(mInAttack[i]>0) // attack
            {
                amp = 1 - mInAttack[i];
                mInAttack[i] -= 1/(mSampleRate * mAttackDuration);
            }
            else if(mInAttack[i]<=0 && mInDecay[i]>0) // decay
            {
                amp = (1-mSustain) * mInDecay[i] + mSustain;
                mInDecay[i] -= 1/(mSampleRate * mDecayDuration);
            }
            else // sustain
            {
                amp = mSustain;
            }
            mBeforeReleaseAmp[i] = amp;
        }
        else 
        {
            if(mInRelease[i]<=0)
            {
                continue;
            }
            else // release
            {
                amp = mBeforeReleaseAmp[i] * mInRelease[i];
                mInRelease[i] -= 1/(mSampleRate*mReleaseDuration);
            }
        }
        auto angle = static_cast<int>(std::floor(currentAngle[i] / (2*PI) * SIZE_WAVEFORM));

        sum += mWaveform[angle] * amp * mGain;
    }
    updateAngle();
    
    if(sum < -1.0f)
    {
        sum = -1.0f;
    }
    if(sum > 1.0f)
    {
        sum = 1.0f;
    }
    return sum;
}

void Synthesizer::updateAngle()
{
    for(int i=0; i<NUM_SYNTH_POLYPHONY; i++)
        if(keyboardOn[i] == true || mInRelease[i]>=0)
        {
            currentAngle[i] += angleDelta[i];
            if (currentAngle[i] >= PI * 2)
                currentAngle[i] -= PI * 2;
        }
}

void Synthesizer::addKeyOn(float tmpAngleDelta)
{
    auto flag = true;
    auto emptyIndex = -1;
    for(int i=0; i<NUM_SYNTH_POLYPHONY; i++)
    {
        if(keyboardOn[i] == false && mInRelease[i] <= 0) // add new key not in release
            emptyIndex = i;
        else if(keyboardOn[i] == false && emptyIndex == -1)
            emptyIndex = i;

        if(isTwoFloatSame(angleDelta[i], tmpAngleDelta)) // pressing the same key multiple times
        {
            flag = false;
            emptyIndex = i;
            break;
        }
    }
    keyboardOn[emptyIndex] = true;
    angleDelta[emptyIndex] = tmpAngleDelta;
    currentAngle[emptyIndex] = 0.0f;
    mInAttack[emptyIndex] = 1.0f;
    mInDecay[emptyIndex] = 1.0f;
    mInRelease[emptyIndex] = 1.0f;
    mBeforeReleaseAmp[emptyIndex] = 0.0f;
}

void Synthesizer::removeKeyOn(float frequency, float sampleRate)
{
    auto cyclesPerSample = frequency / sampleRate;
    auto tmpAngleDelta = cyclesPerSample * PI * 2;

    for(int i=0; i<NUM_SYNTH_POLYPHONY; i++)
    {
        if(isTwoFloatSame(angleDelta[i], tmpAngleDelta))
        {
            keyboardOn[i] = false;
        }
    }
}