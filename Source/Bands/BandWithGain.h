/*
  ==============================================================================

    BandWithGain.h
    Created: 27 Dec 2021 8:49:28am
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Sliders/SliderContainerWithGain.h"

class BandWithGain : public juce::Component
{
public:
    BandWithGain(EQAudioProcessor &p, juce::String filterName, int id)
        : audioProcessor(p),
          slider(p, filterName.toLowerCase().replace(" ", "") + juce::String{id})
    {
        filterId = id;
        label.setText(filterName, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        activate.setClickingTogglesState(true);
        activate.onClick = [this] { updateToggleState(&activate); };
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
        addAndMakeVisible(activate);
    }

    ~BandWithGain() override
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
        activate.setBounds(area.removeFromTop(25));
    }

private:
    EQAudioProcessor &audioProcessor;
    SliderContainerWithGain slider;

    juce::Label label;
    juce::ToggleButton activate { "activate" };

    int filterId;

    void updateToggleState(juce::Button* button)
    {
        juce::String labelText = label.getText().toLowerCase().replace(" ", "");
        if (labelText.compare("lowshelf") == 0) {
            audioProcessor.toggleActivate(4);
        } else if (labelText.compare("highshelf") == 0) {
            audioProcessor.toggleActivate(5);
        } else if (labelText.compare("peak") == 0) {
            audioProcessor.toggleActivate(filterId + 5);
        }
    }
};
