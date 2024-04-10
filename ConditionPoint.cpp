#include "ConditionPoint.h"

ConditionPoint::ConditionPoint()
{
}

void ConditionPoint::mouseDown(const juce::MouseEvent& e)
{
    mDragger.startDraggingComponent(this, e);
}

void ConditionPoint::mouseDrag(const juce::MouseEvent& e)
{
    mDragger.dragComponent(this, e, &mConstrainer);
    Callback(getX(), getY());
}

