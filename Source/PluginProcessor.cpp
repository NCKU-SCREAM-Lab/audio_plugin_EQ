#include "PluginProcessor.h"
#include "PluginEditor.h"
#define MY_PI 3.14159265358979323846

EQAudioProcessor::EQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
        BusesProperties()
        #if ! JucePlugin_IsMidiEffect
            #if ! JucePlugin_IsSynth
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
            #endif
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
        #endif
    ), tree(*this, nullptr, "PARAM", {
        std::make_unique<juce::AudioParameterFloat>(
            "lowpass1_f", "lowpass1_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 10.0f), 16000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "lowpass1_q", "lowpass1_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.01f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "highpass1_f", "highpass1_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 10.0f), 200.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "highpass1_q", "highpass1_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.01f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "notch1_f", "notch1_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 10.0f), 5000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "notch1_q", "notch1_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "notch2_f", "notch2_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 1000.0f), 10000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "notch2_q", "notch2_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "lowshelf1_f", "lowshelf1_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 10.0f), 15000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "lowshelf1_q", "lowshelf1_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "lowshelf1_gain", "lowshelf1_Gain",
            juce::NormalisableRange<float>(1.0f, 10.0f, 0.5f), 2.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "highshelf1_f", "highshelf1_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 10.0f), 15000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "highshelf1_q", "highshelf1_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "highshelf1_gain", "highshelf1_Gain",
            juce::NormalisableRange<float>(1.0f, 10.0f, 0.5f), 2.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "peak1_f", "peak1_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 10.0f), 15000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "peak1_q", "peak1_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "peak1_gain", "peak1_Gain",
            juce::NormalisableRange<float>(1.0f, 10.0f, 0.5f), 2.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "peak2_f", "peak2_F",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 10.0f), 15000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "peak2_q", "peak2_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "peak2_gain", "peak2_Gain",
            juce::NormalisableRange<float>(1.0f, 10.0f, 0.5f), 2.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        )
    }), forwardFFT(fftOrder)
#endif
{
}

EQAudioProcessor::~EQAudioProcessor()
{

}

const juce::String EQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EQAudioProcessor::setCurrentProgram(int index)
{

}

const juce::String EQAudioProcessor::getProgramName(int index)
{
    return {};
}

void EQAudioProcessor::changeProgramName(int index, const juce::String &newName)
{

}

void EQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	overlap = new float[15]{ 0 };
	for (int i = 0; i < fftSize * 2; ++i)
	{
		FIR_freq_response_c.push_back(0.0f);
		FIR_freq_response.push_back(0.0f);
		fftData.push_back(0.0f);
	}

	for (int i = 0; i < fftSize/2; ++i)
	{
		frequencies.push_back(binToFreq(i,sampleRate));
		IIR_Response.push_back(0.5);
		freqResponse.push_back(0.5);
	}
	for (int i = 0; i < 6; i++)
	{
		std::vector<double> newVec;
		IIR_Responses.push_back(newVec);
		for (int j = 0; j < fftSize/2; j++)
		{
			IIR_Responses.at(i).push_back(0.5f);
		}
	}
    singleChannelSampleFifo.prepare(samplesPerBlock);
}

void EQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EQAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EQAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	
	/* White noise for testing */
	/*float value;
	for (int i = 0; i < buffer.getNumSamples(); i++) {
		value = random.nextFloat() * 0.25f - 0.125f;
		value *= 10;

		buffer.setSample(0, i, value);
		buffer.setSample(1, i, value);
	}*/
	
    updateFilters();
    applyFIRFilter(buffer);
    applyIIRFilter(buffer);
    singleChannelSampleFifo.update(buffer);
}

bool EQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *EQAudioProcessor::createEditor()
{
    return new EQAudioProcessorEditor(*this);
}

void EQAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EQAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new EQAudioProcessor();
}

void EQAudioProcessor::updateFilters()
{
    /* update parameters */
    if (activate[0]) {
        if (f[0] != tree.getRawParameterValue("lowpass1_f")->load()) {
            f[0] = tree.getRawParameterValue("lowpass1_f")->load();
            isFIRUpdated = true;
        }
        if (Q[0] != tree.getRawParameterValue("lowpass1_q")->load()) {
            Q[0] = tree.getRawParameterValue("lowpass1_q")->load();
            isFIRUpdated = true;
        }
    }
    if (activate[1]) {
        if (f[1] != tree.getRawParameterValue("highpass1_f")->load()) {
            f[1] = tree.getRawParameterValue("highpass1_f")->load();
            isFIRUpdated = true;
        }
        if (Q[1] != tree.getRawParameterValue("highpass1_q")->load()) {
            Q[1] = tree.getRawParameterValue("highpass1_q")->load();
            isFIRUpdated = true;
        }
    }
    if (activate[2]) {
        if (f[2] != tree.getRawParameterValue("notch1_f")->load()) {
            f[2] = tree.getRawParameterValue("notch1_f")->load();
            isIIRUpdated = true;
        }
        if (Q[2] != tree.getRawParameterValue("notch1_q")->load()) {
            Q[2] = tree.getRawParameterValue("notch1_q")->load();
            isIIRUpdated = true;
        }
    }
    if (activate[3]) {
        if (f[3] != tree.getRawParameterValue("notch2_f")->load()) {
            f[3] = tree.getRawParameterValue("notch2_f")->load();
            isIIRUpdated = true;
        }
        if (Q[3] != tree.getRawParameterValue("notch2_q")->load()) {
            Q[3] = tree.getRawParameterValue("notch2_q")->load();
            isIIRUpdated = true;
        }
    }
    if (activate[4]) {
        if (f[4] != tree.getRawParameterValue("lowshelf1_f")->load()) {
            f[4] = tree.getRawParameterValue("lowshelf1_f")->load();
            isIIRUpdated = true;
        }
        if (Q[4] != tree.getRawParameterValue("lowshelf1_q")->load()) {
            Q[4] = tree.getRawParameterValue("lowshelf1_q")->load();
            isIIRUpdated = true;
        }
        if (gain[0] != tree.getRawParameterValue("lowshelf1_gain")->load()) {
            gain[0] = tree.getRawParameterValue("lowshelf1_gain")->load();
            isIIRUpdated = true;
        }
    }
    if (activate[5]) {
        if (f[5] != tree.getRawParameterValue("highshelf1_f")->load()) {
            f[5] = tree.getRawParameterValue("highshelf1_f")->load();
            isIIRUpdated = true;
        }
        if (Q[5] != tree.getRawParameterValue("highshelf1_q")->load()) {
            Q[5] = tree.getRawParameterValue("highshelf1_q")->load();
            isIIRUpdated = true;
        }
        if (gain[1] != tree.getRawParameterValue("highshelf1_gain")->load()) {
            gain[1] = tree.getRawParameterValue("highshelf1_gain")->load();
            isIIRUpdated = true;
        }
    }
    if (activate[6]) {
        if (f[6] != tree.getRawParameterValue("peak1_f")->load()) {
            f[6] = tree.getRawParameterValue("peak1_f")->load();
            isIIRUpdated = true;
        }
        if (Q[6] != tree.getRawParameterValue("peak1_q")->load()) {
            Q[6] = tree.getRawParameterValue("peak1_q")->load();
            isIIRUpdated = true;
        }
        if (gain[2] != tree.getRawParameterValue("peak1_gain")->load()) {
            gain[2] = tree.getRawParameterValue("peak1_gain")->load();
            isIIRUpdated = true;
        }
    }
    if (activate[7]) {
        if (f[7] != tree.getRawParameterValue("peak2_f")->load()) {
            f[7] = tree.getRawParameterValue("peak2_f")->load();
            isIIRUpdated = true;
        }
        if (Q[7] != tree.getRawParameterValue("peak2_q")->load()) {
            Q[7] = tree.getRawParameterValue("peak2_q")->load();
            isIIRUpdated = true;
        }
        if (gain[3] != tree.getRawParameterValue("peak2_gain")->load()) {
            gain[3] = tree.getRawParameterValue("peak2_gain")->load();
            isIIRUpdated = true;
        }
    }
    
    /* update filters and frequency response */
	if (isFIRUpdated) {
		genFIRFilter();
        isFIRUpdated = false;
	}
	if (isIIRUpdated) {
        genIIRFilter();
        isIIRUpdated = false;
	}

	for (int i = 0; i < fftSize / 2; i++)
		freqResponse[i] = IIR_Response[i] * (double)FIR_freq_response[i];
}

void EQAudioProcessor::genFIRFilter()
{
	int order = qToOrder(Q.at(0)) + qToOrder(Q.at(1));
	/* Reset overlap size */
	delete[] overlap;
	overlap = new float[order] { 0 };

	genAllPass();
    if (activate[0]) genLowPass();
	if (activate[1]) genHighPass();
}

void EQAudioProcessor::genAllPass()
{
	/* Initialize impulse response of All Pass Filter (FIR) */
	int order = qToOrder(2.0f);
	std::array <float, 560> h = { 0.0 };
	h[0] = 1.0f;
	juce::FloatVectorOperations::fill(&h[1], 0.0f, order - 1);

	/* FFT */
	for (int i = 0; i < fftSize * 2; i++)
	{
		FIR_freq_response_c[i] = (i < order) ? h[i] : 0;
		FIR_freq_response[i] = (i < order) ? h[i] : 0;
	}

	/* Create initial all pass filter */
	forwardFFT.performRealOnlyForwardTransform(&FIR_freq_response_c.at(0));
	forwardFFT.performFrequencyOnlyForwardTransform(&FIR_freq_response.at(0));

}

void EQAudioProcessor::genLowPass()
{
	int order = qToOrder(Q.at(0));
	std::vector<float> H, H_freq;
	H.resize(fftSize * 2);
	H_freq.resize(fftSize * 2);
	auto fc = f.at(0) / getSampleRate();
	float impulse_response_sum = 0.0f;

	/* Construct impulse response of Low Pass Filter (FIR) */

	std::array <float, 560> h = { 0.0 };
	for (int i = -(order - 1) / 2, j = 0; i <= order / 2; i++, j++) {
		if (i != 0)
			h[j] = (sin(2 * fc * i * MY_PI) / (2 * fc * i * MY_PI));
		else
			h[j] = (1.0);
	}

	/* Set blackman window */
	w.clear();
	for (int i = 0; i < order; i++)
		w.push_back(0.42 - 0.5 * cos(2 * MY_PI * i / (order - 1)) + 0.08 * cos(4 * MY_PI * i / (order - 1)));

	/* Windowed-Sinc Filter */
	for (int i = 0; i < order; i++) {
		h[i] = h[i] * w.at(i);
		impulse_response_sum += h[i];
	}

	/* Normalized windowed-sinc filter */
	for (int i = 0; i < order; i++)
		h[i] /= impulse_response_sum;

	/* FFT */
	for (int i = 0; i < fftSize * 2; i++)
	{
		H[i] = (i < order) ? h[i] : 0;
		H_freq[i] = (i < order) ? h[i] : 0;
	}
	forwardFFT.performRealOnlyForwardTransform(&H.at(0));
	forwardFFT.performFrequencyOnlyForwardTransform(&H_freq.at(0));

	/* Add new filter into existing one */
	for (int i = 0; i < fftSize * 2; i += 2)
	{
		float r1 = H[i];
		float i1 = H[i + 1];
		float r2 = FIR_freq_response_c[i];
		float i2 = FIR_freq_response_c[i + 1];
		FIR_freq_response_c[i] = r1 * r2 - i1 * i2;
		FIR_freq_response_c[i + 1] = r1 * i2 + i1 * r2;
	}
	juce::FloatVectorOperations::multiply(&FIR_freq_response.at(0), &FIR_freq_response.at(0), &H_freq.at(0), fftSize * 2);
}

void EQAudioProcessor::genHighPass()
{
	int order = qToOrder(Q.at(1));
	std::vector<float> H, H_freq;
	H.resize(fftSize * 2);
	H_freq.resize(fftSize * 2);
	auto fc = f.at(1) / getSampleRate();
	float impulse_response_sum = 0.0f;

	/* Construct impulse response of Low Pass Filter (FIR) first */
	std::array <float, 560> h = { 0.0 };
	for (int i = -(order - 1) / 2, j = 0; i <= order / 2; i++, j++) {
		if (i != 0)
			h.at(j) = (sin(2 * fc * i * MY_PI) / (2 * fc * i * MY_PI));
		else
			h.at(j) = (1.0);
	}

	/* Set blackman window */
	w.clear();
	for (int i = 0; i < order; i++)
		w.push_back(0.42 - 0.5 * cos(2 * MY_PI * i / (order - 1)) + 0.08 * cos(4 * MY_PI * i / (order - 1)));

	/* Windowed-Sinc Filter */
	for (int i = 0; i < order; i++) {
		h.at(i) = h.at(i) * w.at(i);
		impulse_response_sum += h.at(i);
	}

	/* Normalized windowed-sinc filter */
	for (int i = 0; i < order; i++)
		h.at(i) /= impulse_response_sum;

	/* Then using spectral inversion to convert it into a high-pass one. */
	for (int i = 0; i < order; i++)
		h.at(i) *= -1;
	h.at(order / 2) += 1;

	/* FFT */
	for (int i = 0; i < fftSize * 2; i++)
	{
		H[i] = (i < order) ? h[i] : 0;
		H_freq[i] = (i < order) ? h[i] : 0;
	}
	forwardFFT.performRealOnlyForwardTransform(&H.at(0));
	forwardFFT.performFrequencyOnlyForwardTransform(&H_freq.at(0));

	/* Add new filter into existing one */
	for (int i = 0; i < fftSize * 2; i += 2)
	{
		float r1 = H[i];
		float i1 = H[i + 1];
		float r2 = FIR_freq_response_c[i];
		float i2 = FIR_freq_response_c[i + 1];
		FIR_freq_response_c[i] = r1 * r2 - i1 * i2;
		FIR_freq_response_c[i + 1] = r1 * i2 + i1 * r2;
	}
	juce::FloatVectorOperations::multiply(&FIR_freq_response.at(0), &FIR_freq_response.at(0), &H_freq.at(0), fftSize * 2);

}

void EQAudioProcessor::genIIRFilter()
{
	/* generate frequency response of IIR filter */
	std::vector<double> response;
	response.resize(fftSize/2);
    juce::FloatVectorOperations::fill(&IIR_Response[0], 0.5, fftSize / 2);
    coefs = {};

    for (int id = 2; id < 8; ++id) {
        if (!activate[id]) continue;
        auto coef = std::array<float, 6> {0};
        switch (id) {
        case 2: case 3:
            coef = juce::dsp::IIR::ArrayCoefficients<float>::makeNotch(getSampleRate(), f[id], Q[id]);
            break;
        case 4:
            coef = juce::dsp::IIR::ArrayCoefficients<float>::makeLowShelf(getSampleRate(), f[id], Q[id], gain[id - 4]);
            break;
        case 5:
            coef = juce::dsp::IIR::ArrayCoefficients<float>::makeHighShelf(getSampleRate(), f[id], Q[id], gain[id - 4]);
            break;
        case 6: case 7:
            coef = juce::dsp::IIR::ArrayCoefficients<float>::makePeakFilter(getSampleRate(), f[id], Q[id], gain[id - 4]);
            break;
        }
        coefs.push_back(coef);

	    juce::FloatVectorOperations::fill(&response[0], 0, fftSize/2);
        juce::dsp::IIR::Coefficients<float> IIR_filter(coef[0], coef[1], coef[2], coef[3], coef[4], coef[5]);
	    IIR_filter.getMagnitudeForFrequencyArray(&frequencies[0], &response[0], fftSize/2, getSampleRate());
        juce::FloatVectorOperations::copy(&IIR_Responses[id - 2][0], &response[0], fftSize/2);
        juce::FloatVectorOperations::multiply(&IIR_Response[0], &IIR_Responses[id - 2][0], fftSize / 2);
    }
}

void EQAudioProcessor::applyFIRFilter(juce::AudioBuffer<float> &buffer)
{
	juce::FloatVectorOperations::fill(&fftData[0], 0.0f, fftSize * 2);

    auto *readIndex = buffer.getReadPointer(0);
	juce::FloatVectorOperations::copy(&fftData[0], readIndex, buffer.getNumSamples());

	/* signal FFT */
    forwardFFT.performRealOnlyForwardTransform(&fftData.at(0));
	/* Dot product with the frequency response of the filter */
	for (int i = 0; i < fftSize * 2; i += 2)
	{
		float sr = fftData[i];
		float si = fftData[i + 1];
		float fr = FIR_freq_response_c[i];
		float fi = FIR_freq_response_c[i + 1];
		fftData[i] = sr * fr - si * fi;
		fftData[i + 1] = sr * fi + si * fr;
	}
	/* signal iFFT */
	forwardFFT.performRealOnlyInverseTransform(&fftData.at(0));

	/* Overlap-Add method */
	int order = qToOrder(Q.at(0)) + qToOrder(Q.at(1));
	for (int i = 0; i < order - 1; i++)
	{
		fftData[i] += overlap[i];
	}
	for (int i = 0; i<buffer.getNumSamples(); i++) {
		buffer.setSample(0, i, fftData[i]);
		buffer.setSample(1, i, fftData[i]);
	}
	for (int i = 0; i < order - 1; i++)
	{
		overlap[i] = fftData[buffer.getNumSamples() + i];
	}
}

void EQAudioProcessor::applyIIRFilter(juce::AudioBuffer<float> &buffer)
{
    float value;
    std::vector <float> x;  // input signal
	std::vector <float> y;  // output signal
    std::vector <float> h1; // impulse response for input
    std::vector <float> h2; // impulse response for output

    /* Call JUCE API to get coefficients of the corresponding filter */
    for (int j = 0; j < coefs.size(); ++j) {
        /* initialize the first few input and output signal to 0 */
        x = {0, 0, 0};
        y = {0, 0};

        /* construct impulse response */
        auto coef = coefs[j];
        h1 = { coef[2] / coef[3], coef[1] / coef[3], coef[0] / coef[3] }; 
        h2 = { coef[5] / coef[3], coef[4] / coef[3] };

        /* apply IIR filter */
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            x.erase(x.begin());
            x.push_back(buffer.getSample(0, i));

            value = 0.0;
            for (int j = 0; j < 3; j++)
                value += x.at(j) * h1.at(j);
            for (int j = 0; j < 2; j++)
                value -= y.at(j) * h2.at(j);

            y.erase(y.begin());
            y.push_back(value);

            buffer.setSample(0, i, value);
            buffer.setSample(1, i, value);
        }
    }
}

int EQAudioProcessor::qToOrder(float q)
{
	auto map = [](float q)
	{
		return juce::jmap(q,
			0.1f, 2.0f,
			15.f, 255.f);
	};

	return ((int)map(q) % 2 == 1) ? (int)map(q) : (int)map(q) - 1;
}

float EQAudioProcessor::binToFreq(int binNum, double sr)
{
	float binWidth = sr / fftSize;
	auto binFreq = binNum * binWidth;
	return binFreq;
}
