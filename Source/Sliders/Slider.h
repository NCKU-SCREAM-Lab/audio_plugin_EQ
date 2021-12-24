/*
  ==============================================================================

    Slider.h
    Created: 24 Dec 2021 10:09:48pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "MyLookAndFeel.h"

class Slider : public juce::Component
{
public:
    Slider(EQAudioProcessor &, juce::String, juce::String);
    ~Slider() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    EQAudioProcessor &audioProcessor;
    juce::Slider slider;
    juce::Label label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    MyLookAndFeel lnf;
};
