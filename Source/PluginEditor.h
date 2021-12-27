#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FrequencySpectrum.h"
#include "Bands/LowPassBand.h"
#include "Bands/HighPassBand.h"
#include "Bands/NotchBand.h"
#include "Bands/LowShelfBand.h"
#include "Bands/HighShelfBand.h"
#include "Bands/PeakBand.h"

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
    LowPassBand lowpass1;
    HighPassBand highpass1;
    NotchBand notch1, notch2;
    LowShelfBand lowshelf1;
    HighShelfBand highshelf1;
    PeakBand peak1, peak2;

    std::vector<juce::Component *> subcomponents {
        &spectrum,
        &lowpass1, &highpass1, &notch1, &notch2,
        &lowshelf1, &highshelf1, &peak1, &peak2
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQAudioProcessorEditor)
};
