/*
  ==============================================================================

    Slider.cpp
    Created: 24 Dec 2021 10:09:48pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Slider.h"

Slider::Slider(EQAudioProcessor &p, juce::String labelName, juce::String filterName)
    : audioProcessor(p)
{
    setLookAndFeel(&lnf);
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, slider.getTextBoxWidth(), slider.getTextBoxHeight());
    label.setText(labelName, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.tree, juce::String{filterName}.toLowerCase() + "_" + juce::String{labelName}.toLowerCase(), slider
    ));
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

Slider::~Slider()
{
    setLookAndFeel(nullptr);
}

void Slider::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void Slider::resized()
{
    auto area = getLocalBounds();
    label.setBounds(area.removeFromTop(15));
    slider.setBounds(area.reduced(4));
}
