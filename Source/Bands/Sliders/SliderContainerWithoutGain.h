/*
  ==============================================================================

    SliderContainerWithoutGain.h
    Created: 24 Dec 2021 10:12:21pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "Slider.h"

class SliderContainerWithoutGain : public juce::Component
{
public:
    SliderContainerWithoutGain(EQAudioProcessor &p, juce::String filterName)
        : audioProcessor(p),
          fSlider(p, "f", filterName),
          QSlider(p, "q", filterName)
    {    
        for (auto &slider : sliders) {
            addAndMakeVisible(slider);
        }
    }

    ~SliderContainerWithoutGain() override
    {

    }

    void paint(juce::Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        g.setColour(juce::Colours::grey);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 3, 3);
    }

    void resized() override
    {
        juce::FlexBox flexbox;
        flexbox.flexDirection = juce::FlexBox::Direction::column;
        for (auto &slider : sliders) {
            juce::FlexItem item {*slider};
            flexbox.items.add(item.withFlex(1.0));
        }
        flexbox.performLayout(getLocalBounds());
        for (auto &slider : sliders) {
            slider->setBounds(slider->getBounds().reduced(3));
        }
    }

private:
    EQAudioProcessor &audioProcessor;
    Slider fSlider, QSlider;

    std::vector<Slider *> sliders { &fSlider, &QSlider };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderContainerWithoutGain);
};
