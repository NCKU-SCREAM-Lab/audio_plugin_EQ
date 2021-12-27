/*
  ==============================================================================

    LowShelfBand.h
    Created: 27 Dec 2021 8:04:19am
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Sliders/SliderContainerWithGain.h"

class LowShelfBand : public juce::Component
{
public:
    LowShelfBand(EQAudioProcessor &p, juce::String filterName, int id)
        : audioProcessor(p),
          slider(p, filterName.toLowerCase().replace(" ", "") + juce::String{id})
    {
        label.setText(filterName, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
    }

    ~LowShelfBand() override
    {
        
    }

    void paint(juce::Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto area = getLocalBounds();
        label.setBounds(area.removeFromTop(15));
        slider.setBounds(area.reduced(4));
    }

private:
    EQAudioProcessor &audioProcessor;
    SliderContainerWithGain slider;

    juce::Label label;
};