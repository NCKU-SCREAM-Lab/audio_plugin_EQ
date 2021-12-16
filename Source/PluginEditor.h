#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class EQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    EQAudioProcessorEditor(EQAudioProcessor &);
    ~EQAudioProcessorEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    EQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQAudioProcessorEditor)
};
