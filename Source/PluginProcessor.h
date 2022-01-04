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

	//std::vector<float> getFilterResponse() { return FIR_freq_response; }
	std::vector<double> getFilterResponse() { return freqResponse; }

    void toggleActivate(int index)
    { 
        activate[index] = !activate[index];
        if (index < 2)
            isFIRUpdated = true;
        else
            isIIRUpdated = true;
    }

private:
    SingleChannelSampleFifo<juce::AudioBuffer<float>> singleChannelSampleFifo {0};

    /* Order: [lowpass1, highpass1, notch1, notch2, ]lowshelf1, highshelf1, peak1, peak2 */
    std::vector<bool> activate { false, false, false, false, false, false, false, false };
    std::vector<int> f { 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<float> Q { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    std::vector<float> gain { 0.0f, 0.0f, 0.0f, 0.0f };

	float* overlap;
	juce::dsp::FFT forwardFFT;
	std::vector<float> w;                    // window function
	std::vector<float> fftData;
	std::vector<double> frequencies;
	std::vector<float> FIR_freq_response_c;  // FIR frequency response with complex value
	std::vector<float> FIR_freq_response;    // FIR frequency response with only real value
	std::vector<double> IIR_Response;
	std::vector<std::vector<double>> IIR_Responses;
	std::vector<double> freqResponse;

	juce::Random random;

    void updateFilters();
	bool isFIRUpdated = true;
	bool isIIRUpdated = false;
    
    void genFIRFilter();
	void genAllPass();
	void genLowPass();
	void genHighPass();

	std::array<float, 6> genIIRFilter(int);

    void applyFIRFilter(juce::AudioBuffer<float> &);
    void applyIIRFilter(juce::AudioBuffer<float> &);

	int qToOrder(float);
	float binToFreq(int, double);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQAudioProcessor);
};
