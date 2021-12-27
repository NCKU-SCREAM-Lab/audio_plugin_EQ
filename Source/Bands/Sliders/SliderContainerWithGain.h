/*
  ==============================================================================

    SliderContainerWithGain.h
    Created: 24 Dec 2021 10:12:08pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "Slider.h"

class SliderContainerWithGain : public juce::Component
{
public:
    SliderContainerWithGain(EQAudioProcessor &p, juce::String filterName)
        : audioProcessor(p),
          fSlider(p, "f", filterName),
          QSlider(p, "q", filterName),
          gainSlider(p, "gain", filterName)
    {    
        for (auto &slider : sliders) {
            addAndMakeVisible(slider);
        }
    }

    ~SliderContainerWithGain() override
    {

    }

    void paint(juce::Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        juce::FlexBox flexBox;
        for (auto &slider : sliders) {
            juce::FlexItem item {*slider};
            flexBox.items.add(item.withFlex(1.0));
        }
        flexBox.performLayout(getLocalBounds());
        for (auto &slider : sliders) {
            slider->setBounds(slider->getBounds().reduced(5));
        }
    }

private:
    EQAudioProcessor &audioProcessor;
    Slider fSlider, QSlider, gainSlider;

    std::vector<Slider *> sliders { &fSlider, &QSlider, &gainSlider };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderContainerWithGain);
};
