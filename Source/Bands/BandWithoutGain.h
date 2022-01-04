/*
  ==============================================================================

    BandWithoutGain.h
    Created: 27 Dec 2021 8:49:49am
    Author:  robert0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Sliders/SliderContainerWithoutGain.h"

class BandWithoutGain : public juce::Component
{
public:
    BandWithoutGain(EQAudioProcessor &p, juce::String filterName, int id)
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

    ~BandWithoutGain() override
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
    SliderContainerWithoutGain slider;

    juce::Label label;
    juce::ToggleButton activate { "activate" };

    int filterId;

    void updateToggleState(juce::Button* button)
    {
        juce::String labelText = label.getText().toLowerCase().replace(" ", "");
        if (labelText.compare("lowpass") == 0) {
            audioProcessor.toggleActivate(0);
        } else if (labelText.compare("highpass") == 0) {
            audioProcessor.toggleActivate(1);
        } else if (labelText.compare("notch") == 0) {
            audioProcessor.toggleActivate(filterId + 1);
        }
    }
};
