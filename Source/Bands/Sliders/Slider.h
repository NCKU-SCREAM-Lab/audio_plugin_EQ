/*
  ==============================================================================

    Slider.h
    Created: 24 Dec 2021 10:09:48pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "MyLookAndFeel.h"

class Slider : public juce::Component
{
public:
    Slider(EQAudioProcessor &p, juce::String labelName, juce::String filterID)
        : audioProcessor(p)
    {
        setLookAndFeel(&lnf);
        slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        slider.setTextBoxStyle(juce::Slider::TextBoxRight, true, slider.getTextBoxWidth(), slider.getTextBoxHeight());
        label.setText(labelName, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            audioProcessor.tree, juce::String{filterID} + "_" + juce::String{labelName}.toLowerCase(), slider
        ));
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
    }

    ~Slider() override
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto area = getLocalBounds();
        juce::FlexBox flexbox;
        flexbox.flexDirection = juce::FlexBox::Direction::row;
        flexbox.items.add(juce::FlexItem(label).withFlex(1.0f));
        flexbox.items.add(juce::FlexItem(slider).withFlex(2.0f));
        flexbox.performLayout(area.reduced(10));
    }

private:
    EQAudioProcessor &audioProcessor;
    juce::Slider slider;
    juce::Label label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    MyLookAndFeel lnf;
};
