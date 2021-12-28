#pragma once

#include <JuceHeader.h>
#include "FIFO.h"

class EQAudioProcessor : public juce::AudioProcessor
{
public:
    EQAudioProcessor();
    ~EQAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    SingleChannelSampleFifo<juce::AudioBuffer<float>> &getSingleChannelSampleFifo()
    {
        return singleChannelSampleFifo;
    }

    juce::AudioProcessorValueTreeState tree;

	static constexpr auto fftOrder = 10;
	static constexpr auto fftSize = 1 << fftOrder;

	std::vector<float> getFilterResponse() { return H_freq_total; }

private:
    SingleChannelSampleFifo<juce::AudioBuffer<float>> singleChannelSampleFifo {0};

    /* Order: [lowpass1, highpass1, notch1, notch2, ]lowshelf1, highshelf1, peak1, peak2 */
    std::vector<int> f { 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<float> Q { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    std::vector<float> gain { 0.0f, 0.0f, 0.0f, 0.0f };

	std::array <float, 560> h = { 0.0 };
	std::vector<float> w;  // window function
	std::vector<float> fftData;
	std::vector<float> H;
	std::vector<float> H_freq;
	std::vector<float> H_total;
	std::vector<float> H_freq_total;

	float* overlap;
	juce::dsp::FFT forwardFFT;

	juce::Random random;

    void updateParameters();
    void genFilter();
	void genAllPass();
	void genLowPass();
	void genHighPass();

	int qToOrder(float q);



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQAudioProcessor);
};
