#include "Thumb.h"

Thumb::Thumb()
{
    mConstrainer.setMinimumOnscreenAmounts(THUMBWIDTH, THUMBWIDTH, THUMBWIDTH, THUMBWIDTH);
}

void Thumb::mouseDown(const juce::MouseEvent& e)
{
    mDragger.startDraggingComponent(this, e);
}

void Thumb::mouseDrag(const juce::MouseEvent& e)
{
    mDragger.dragComponent(this, e, &mConstrainer);
}

void Thumb::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.fillEllipse(getLocalBounds().toFloat());
}

void Thumb::resized()
{
}
