#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FrequencySpectrum.h"
#include "Bands/BandWithoutGain.h"
#include "Bands/BandWithGain.h"

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
    BandWithoutGain lowpass1, highpass1, notch1, notch2;
    BandWithGain lowshelf1, highshelf1, peak1, peak2;

    std::vector<juce::Component *> subcomponents {
        &spectrum,
        &lowpass1, &highpass1, &notch1, &notch2,
        &lowshelf1, &highshelf1, &peak1, &peak2
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQAudioProcessorEditor)
};
