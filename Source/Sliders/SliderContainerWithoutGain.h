/*
  ==============================================================================

    SliderContainerWithoutGain.h
    Created: 24 Dec 2021 10:12:21pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Slider.h"

class SliderContainerWithoutGain : public juce::Component
{
public:
    SliderContainerWithoutGain(EQAudioProcessor &, juce::String);
    ~SliderContainerWithoutGain() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    EQAudioProcessor &audioProcessor;
    Slider fSlider, QSlider;

    std::vector<Slider *> sliders { &fSlider, &QSlider };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderContainerWithoutGain);
};
