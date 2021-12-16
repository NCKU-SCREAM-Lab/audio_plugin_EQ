/*
  ==============================================================================

    FrequencySpectrum.h
    Created: 17 Dec 2021 5:18:47am
    Author:  robertlin0401, abao1005

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FIFO.h"

class FrequencySpectrum : public juce::Component, public juce::Timer
{
public:
    FrequencySpectrum(EQAudioProcessor &);
    ~FrequencySpectrum() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void timerCallback() override;

private:
    EQAudioProcessor &audioProcessor;
    SingleChannelSampleFifo<juce::AudioBuffer<float>> &spectrum;
    juce::AudioBuffer<float> pathBuffer;
    FFTDataGenerator<std::vector<float>> fftDataGenerator;
    FFTPathProducer<juce::Path> pathProducer;
    juce::Path spectrumPath;

    void generateSpectrum();
    void drawBackgroundGrid(juce::Graphics &);
    void drawTextLabels(juce::Graphics &);

    std::vector<float> getFrequencies()
    {
        return std::vector<float> {
            20  , 50  , 100  , 
            200 , 500 , 1000 ,
            2000, 5000, 10000,
            20000
        };
    }

    std::vector<float> getGains()
    {
        return std::vector<float> {
            -36, -24, -12, 0, 12
        };
    }

    std::vector<float> getXs(const std::vector<float> &freqs, float left, float width)
    {
        std::vector<float> xs;
        for (auto f : freqs) {
            auto normX = juce::mapFromLog10(f, 20.f, 20000.f);
            xs.push_back(left + width * normX);
        }
        return xs;
    }

    juce::Rectangle<int> getRenderArea()
    {
        auto bounds = getLocalBounds();
        bounds.removeFromTop(3);
        bounds.removeFromBottom(3);
        bounds.removeFromLeft(3);
        bounds.removeFromRight(3);
        return bounds;
    }

    juce::Rectangle<int> getAnalysisArea()
    {
        auto bounds = getLocalBounds();
        bounds.removeFromTop(14);
        bounds.removeFromBottom(18);
        bounds.removeFromLeft(18);
        bounds.removeFromRight(16);
        return bounds;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencySpectrum);
};
