/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavespaceAudioProcessorEditor::WavespaceAudioProcessorEditor(WavespaceAudioProcessor &p)
    : AudioProcessorEditor(&p), mAudioProcessor(p), 
    mAttachmentSliderStyleType(p.apvts, "TYPE_STYLE", mSliderStyleType),
    mAttachmentSliderDescriptorType(p.apvts, "TYPE_DESCRIPTOR", mSliderDescriptorType),
    mAttachmentGain(p.apvts, "gain", mSliderGain),
    mAttachmentAttack(p.apvts, "attack", mSliderAttack),
    mAttachmentDecay(p.apvts, "decay", mSliderDecay),
    mAttachmentSustain(p.apvts, "sustain", mSliderSustain),
    mAttachmentRelease(p.apvts, "release", mSliderRelease),

    mAttachmentSliderStyleX0(p.apvts, "STYLE_X_0", mSliderStyleX[0]),
    mAttachmentSliderStyleX1(p.apvts, "STYLE_X_1", mSliderStyleX[1]),
    mAttachmentSliderStyleY0(p.apvts, "STYLE_Y_0", mSliderStyleY[0]),
    mAttachmentSliderStyleY1(p.apvts, "STYLE_Y_1", mSliderStyleY[1]),
    mAttachmentSliderDescriptorX0(p.apvts, "DESCRIPTOR_X_0", mSliderDescriptorX[0]),
    mAttachmentSliderDescriptorX1(p.apvts, "DESCRIPTOR_X_1", mSliderDescriptorX[1]),
    mAttachmentSliderDescriptorX2(p.apvts, "DESCRIPTOR_X_2", mSliderDescriptorX[2]),
    mAttachmentSliderDescriptorX3(p.apvts, "DESCRIPTOR_X_3", mSliderDescriptorX[3]),
    mAttachmentSliderDescriptorX4(p.apvts, "DESCRIPTOR_X_4", mSliderDescriptorX[4]),

    mPadStyle(PadData{NUM_STYLE, "STYLE", STYLE_CONDITION_MIN, STYLE_CONDITION_MAX, &mSliderStyleOnScreenX, &mSliderStyleOnScreenY, this}),
    mPadDescriptor(PadData{NUM_DESCRIPTOR, "DESCRIPTOR", DESCRIPTOR_CONDITION_MIN, DESCRIPTOR_CONDITION_MAX, &mSliderDescriptorOnScreenX, nullptr, this}),
    mPadWaveform(p.mSynth.mWaveform.data())
{
  addAndMakeVisible(mPadStyle);
  addAndMakeVisible(mPadDescriptor);
  addAndMakeVisible(mPadWaveform);

  addAndMakeVisible(mSliderGain);
  mSliderGain.addListener(this);
  addAndMakeVisible(mSliderAttack);
  mSliderAttack.addListener(this);
  addAndMakeVisible(mSliderDecay);
  mSliderDecay.addListener(this);
  addAndMakeVisible(mSliderSustain);
  mSliderSustain.addListener(this);
  addAndMakeVisible(mSliderRelease);
  mSliderRelease.addListener(this);

  addAndMakeVisible(mGainTextBox);
  mGainTextBox.setText("Gain");
  addAndMakeVisible(mAttackTextBox);
  mAttackTextBox.setText("Attack");
  addAndMakeVisible(mDecayTextBox);
  mDecayTextBox.setText("Decay");
  addAndMakeVisible(mSustainTextBox);
  mSustainTextBox.setText("Sustain");
  addAndMakeVisible(mReleaseTextBox);
  mReleaseTextBox.setText("Release");

  mSliderDescriptorOnScreenX.addListener(this);
  addChildComponent(mSliderDescriptorOnScreenX);
  mSliderStyleOnScreenX.addListener(this);
  addChildComponent(mSliderStyleOnScreenX);
  mSliderStyleOnScreenY.addListener(this);
  addChildComponent(mSliderStyleOnScreenY);

  mSliderStyleType.addListener(this);
  addAndMakeVisible(mSliderStyleType);
  mSliderDescriptorType.addListener(this);
  addAndMakeVisible(mSliderDescriptorType);

  addAndMakeVisible(mStyleTextBox);
  addAndMakeVisible(mDescriptorTextBox);

  for(int i=0; i<NUM_STYLE; i++)
  { 
    addChildComponent(mSliderStyleX[i]);
    mSliderStyleX[i].addListener(this);
    addChildComponent(mSliderStyleY[i]);
    mSliderStyleY[i].addListener(this);
  }
  for(int i=0; i<NUM_DESCRIPTOR; i++)
  {
    addChildComponent(mSliderDescriptorX[i]);
    mSliderDescriptorX[i].addListener(this);

  }

  setSize(960, 500);
  setResizable(false, false);

  //set slider values
  setValues();
  mStyleTextBox.setText(NAMES_STYLE[mAudioProcessor.mCurConditionType.curTypeStyle]);
  mDescriptorTextBox.setText(NAMES_DESCRIPTOR[mAudioProcessor.mCurConditionType.curTypeDescriptor]);
}

WavespaceAudioProcessorEditor::~WavespaceAudioProcessorEditor()
{
}

//==============================================================================
void WavespaceAudioProcessorEditor::paint(juce::Graphics &g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
}

void WavespaceAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();

   //LEFT SIDE
  auto sliderStyleTypeBound = area.removeFromLeft(30);
  mSliderStyleType.setBounds(sliderStyleTypeBound);
  auto padStyleBound = createSquareFromRectangle(&area);
  mPadStyle.setBounds(area.removeFromLeft(padStyleBound.getWidth()));
  
  //RIGHT SIDE
  auto WTBound = area.removeFromTop(area.getHeight() * 1 / 2);
  mPadWaveform.setBounds(WTBound);
  auto padDescriptorBound = area.removeFromTop(THUMBWIDTH);
  mPadDescriptor.setBounds(padDescriptorBound);
  auto sliderDescriptorTypeBound = area.removeFromTop(30);
  mSliderDescriptorType.setBounds(sliderDescriptorTypeBound);
  
  auto textBoxesBound = area.removeFromTop(area.getHeight()*0.2);
  mStyleTextBox.setBounds(textBoxesBound.removeFromLeft(textBoxesBound.getWidth()/2));
  mDescriptorTextBox.setBounds(textBoxesBound);
  
  auto AmpEnvBound = area.removeFromTop(area.getHeight() * 0.8);
  auto SliderWidth = AmpEnvBound.getWidth()*0.2;
  mSliderGain.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
  mSliderAttack.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
  mSliderDecay.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
  mSliderSustain.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
  mSliderRelease.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));

  auto AmpTextBoxBound = area;
  mGainTextBox.setBounds(AmpTextBoxBound.removeFromLeft(SliderWidth));
  mAttackTextBox.setBounds(AmpTextBoxBound.removeFromLeft(SliderWidth));
  mDecayTextBox.setBounds(AmpTextBoxBound.removeFromLeft(SliderWidth));
  mSustainTextBox.setBounds(AmpTextBoxBound.removeFromLeft(SliderWidth));
  mReleaseTextBox.setBounds(AmpTextBoxBound.removeFromLeft(SliderWidth));
}

void WavespaceAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
  if(slider == &mSliderStyleType)
  {
    int idx = slider->getValue();
    mStyleTextBox.setText(NAMES_STYLE[idx]);
    mSliderStyleOnScreenX.setValue(mSliderStyleX[idx].getValue());
    mSliderStyleOnScreenY.setValue(mSliderStyleY[idx].getValue());
    return;
  }
  if(slider == &mSliderDescriptorType)
  {
    int idx = slider->getValue();
    mDescriptorTextBox.setText(NAMES_DESCRIPTOR[idx]);
    mSliderDescriptorOnScreenX.setValue(mSliderDescriptorX[idx].getValue());
    return;
  }
  if(slider == &mSliderStyleOnScreenX)
  {
    int idx = mSliderStyleType.getValue();
    float newValue = slider->getValue();

    mSliderStyleX[idx].setValue(newValue);
    mPadStyle.sliderValueChanged("x", newValue);
    return;
  }
  if(slider == &mSliderStyleOnScreenY)
  {
    int idx = mSliderStyleType.getValue();
    float newValue = slider->getValue();

    mSliderStyleY[idx].setValue(newValue);
    mPadStyle.sliderValueChanged("y", newValue);
    return;
  }
  if(slider == &mSliderDescriptorOnScreenX)
  {
    int idx = mSliderDescriptorType.getValue();
    float newValue = slider->getValue();

    mSliderDescriptorX[idx].setValue(newValue);
    mPadDescriptor.sliderValueChanged("x", newValue);
    return;
  }

  // cond Value slider changed
  // check only for condType; if then, thumb has to change
  int condType = mSliderStyleType.getValue();
  if(slider == &mSliderStyleX[condType])
  {
    mSliderStyleOnScreenX.setValue(slider->getValue());
    sliderValueChanged(&mSliderStyleOnScreenX);
    return;
  }
  if(slider == &mSliderStyleY[condType])
  {
    mSliderStyleOnScreenY.setValue(slider->getValue());
    sliderValueChanged(&mSliderStyleOnScreenY);
    return;
  }
  condType = mSliderDescriptorType.getValue();
  if(slider == &mSliderDescriptorX[condType])
  {
    mSliderDescriptorOnScreenX.setValue(slider->getValue());
    sliderValueChanged(&mSliderDescriptorOnScreenX);
    return;
  }

}

void WavespaceAudioProcessorEditor::setValues()
{
  int condType = mSliderStyleType.getValue();
  sliderValueChanged(&mSliderStyleX[condType]);
  sliderValueChanged(&mSliderStyleY[condType]);

  condType = mSliderDescriptorType.getValue();
  sliderValueChanged(&mSliderDescriptorX[condType]);
}