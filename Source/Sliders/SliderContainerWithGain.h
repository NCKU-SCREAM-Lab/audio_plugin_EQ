/*
  ==============================================================================

    SliderContainerWithGain.h
    Created: 24 Dec 2021 10:12:08pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Slider.h"

class SliderContainerWithGain : public juce::Component
{
public:
    SliderContainerWithGain(EQAudioProcessor &, juce::String);
    ~SliderContainerWithGain() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    EQAudioProcessor &audioProcessor;
    Slider fSlider, QSlider, gainSlider;

    std::vector<Slider *> sliders { &fSlider, &QSlider, &gainSlider };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderContainerWithGain);
};
