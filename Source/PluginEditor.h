#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FrequencySpectrum.h"
#include "Bands/Sliders/SliderContainerWithoutGain.h"
#include "Bands/Sliders/SliderContainerWithGain.h"

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
    SliderContainerWithoutGain testSliderWithoutGain;
    SliderContainerWithGain testSliderWithGain;

    std::vector<juce::Component *> subcomponents { &spectrum, &testSliderWithoutGain, &testSliderWithGain };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQAudioProcessorEditor)
};
