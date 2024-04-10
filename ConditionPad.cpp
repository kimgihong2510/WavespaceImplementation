#include "ConditionPad.h"

ConditionPad::ConditionPad(PadData padData)
{
    mPadData = padData;

    addAndMakeVisible(mConditionPoint);
    mConditionPoint.Callback = [&](float posX, float posY)
    {
        auto parentWidth = getLocalBounds().toFloat().getWidth();
        auto parentHeight = getLocalBounds().toFloat().getHeight();
        auto thumbWidth = static_cast<float>(THUMBWIDTH);

        posY = (getHeight()-thumbWidth) - posY;
        auto mCondValueX = juce::jmap(posX, 0.0f, parentWidth - thumbWidth, mPadData.COND_MIN, mPadData.COND_MAX);
        mPadData.sliderX->setValue(mCondValueX);

        if(mPadData.sliderY != nullptr)
        {
            auto mCondValueY = juce::jmap(posY, 0.0f, parentHeight - thumbWidth, mPadData.COND_MIN, mPadData.COND_MAX);
            mPadData.sliderY->setValue(mCondValueY);
        }
    };
}


void ConditionPad::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    //g.drawRoundedRectangle(bounds, 10, 7);
    g.setGradientFill(juce::ColourGradient{ juce::Colours::black.brighter(0.2f), bounds.getTopLeft(), juce::Colours::black.brighter(0.1f), bounds.getBottomLeft(), false });
    g.fillRoundedRectangle(bounds, 10);
    
}
void ConditionPad::resized()
{
    mConditionPoint.setBounds(getLocalBounds().withSizeKeepingCentre(THUMBWIDTH, THUMBWIDTH));
}

void ConditionPad::sliderValueChanged(juce::String type, float value)
{
    int newPosX, newPosY;
    auto thumbWidth = static_cast<float>(THUMBWIDTH);
    auto bounds = getLocalBounds().toFloat();
    if(type.equalsIgnoreCase("x"))
    {
        newPosX = static_cast<int>(juce::jmap(value, mPadData.COND_MIN, mPadData.COND_MAX, 0.0f, bounds.getWidth() - thumbWidth));
        newPosY = mConditionPoint.getY();
    }
    else
    {
        newPosX = mConditionPoint.getX();
        newPosY = (bounds.getHeight() - thumbWidth) - static_cast<int>(juce::jmap(value, mPadData.COND_MIN, mPadData.COND_MAX, 0.0f , bounds.getHeight() - thumbWidth));
    }
    mConditionPoint.setTopLeftPosition(newPosX, newPosY);        
}

void ConditionPad::mouseDown(const juce::MouseEvent& e)
{
    int posX = e.getMouseDownX() - THUMBWIDTH / 2;
    int posY = e.getMouseDownY() - THUMBWIDTH / 2;
    if(posX<0){posX = 0;}
    else if(posX>getLocalBounds().getWidth() - THUMBWIDTH) {posX = getLocalBounds().getWidth() - THUMBWIDTH;}
    if(posY<0){posY = 0;}
    else if(posY>getLocalBounds().getHeight() - THUMBWIDTH) {posY = getLocalBounds().getHeight() - THUMBWIDTH;}

    mConditionPoint.setTopLeftPosition(posX , posY);
    mConditionPoint.Callback(posX, posY); 

    mConditionPoint.mouseDown(e);
}

void ConditionPad::mouseDrag(const juce::MouseEvent& e)
{
    mConditionPoint.mouseDrag(e);
}