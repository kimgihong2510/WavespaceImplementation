#include "TextComponent.h"

void TextComponent::paint(juce::Graphics& g) 
{
    g.setFont(24.0f);
    g.setColour(juce::Colours::white);
    g.drawText(mText, getLocalBounds(), juce::Justification::centred, true);
}

void TextComponent::setText(const juce::String& newText)
{
    mText = newText;
    repaint();
}
