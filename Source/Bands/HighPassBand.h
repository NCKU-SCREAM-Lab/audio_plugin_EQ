/*
  ==============================================================================

    HighPassBand.h
    Created: 27 Dec 2021 8:04:08am
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Sliders/SliderContainerWithoutGain.h"

class HighPassBand : public juce::Component
{
public:
    HighPassBand(EQAudioProcessor &p, juce::String filterName, int id)
        : audioProcessor(p),
          slider(p, filterName.toLowerCase().replace(" ", "") + juce::String{id})
    {
        label.setText(filterName, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
    }

    ~HighPassBand() override
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
    SliderContainerWithoutGain slider;

    juce::Label label;
};
