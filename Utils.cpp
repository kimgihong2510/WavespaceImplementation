#include "Utils.h"

/*juce::String makeParamName(juce::String condType, juce::String pointType)
{
    return condType + "_" + pointType;
}
juce::String makeParamName(juce::String condType, juce::String pointType, int numCond)
{
    return condType + "_" + pointType + "_" + juce::String(numCond);
}
*/

juce::StringArray splitString(const juce::String& text, const juce::String& delimiter)
{
    juce::StringArray result;

    int startIndex = 0;
    int endIndex = text.indexOf(delimiter);

    while (endIndex != -1)
    {
        if(startIndex<endIndex)
        {
            result.add(text.substring(startIndex, endIndex));
        }
        startIndex = endIndex + delimiter.length();
        endIndex = text.indexOf(startIndex, delimiter);
    }

    // Add the remaining part of the string
    result.add(text.substring(startIndex));

    return result;
}


void performIFFT(juce::dsp::FFT& fft, const float* ampArray, const float* phaseArray, std::array<float, SIZE_WAVEFORM*2>& fftArray)
{
    std::complex<float> _j(0.0f, 1.0f);

    fftArray[0] = fftArray[1] = 0.0f; // zero DC
    for (int i = 1; i <= SIZE_WAVEFORM/2; ++i)
    {
        auto tmpZ = performPolar(ampArray[i-1], phaseArray[i-1]) * _j;
        fftArray[2*i] = tmpZ.real();
        fftArray[2*i+1] = tmpZ.imag();
    }

    fft.performRealOnlyInverseTransform(fftArray.data());
}

inline std::complex<float> performPolar(float amp, float phase)
{
    if(amp<0)
    {
        amp = -amp;
        phase += PI;
    }
    return std::polar(amp, phase);
}

juce::String makeParamID(juce::String condType, juce::String pointType, int numCond)
{
    return condType + "_" + pointType + "_" + juce::String(numCond);
}

juce::Rectangle<int> createSquareFromRectangle(juce::Rectangle<int>* bounds)
{
    int squareSize = std::min(bounds->getWidth(), bounds->getHeight());
    
    juce::Rectangle<int> square(bounds->getX(), bounds->getY(), squareSize, squareSize);
    return square;
}

bool isTwoFloatSame(float a, float b)
{
    float epsilon = static_cast<float>(1e-4);
    if(abs(a-b) < epsilon)
        return true;
    else
        return false;
}