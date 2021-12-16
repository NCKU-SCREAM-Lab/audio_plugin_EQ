#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FrequencySpectrum.h"

class EQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    EQAudioProcessorEditor(EQAudioProcessor &);
    ~EQAudioProcessorEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    EQAudioProcessor& audioProcessor;
    FrequencySpectrum spectrum;

    std::vector<juce::Component *> subcomponents { &spectrum };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQAudioProcessorEditor)
};
