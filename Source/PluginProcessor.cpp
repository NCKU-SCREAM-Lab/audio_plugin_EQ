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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 10.0f), 15000.0f,
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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 10.0f), 200.0f,
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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
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
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
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
	overlap = new float[15]{ 0 };
	for (int i = 0; i < fftSize*2; ++i)
	{
		H.push_back(0.0f);
		H_freq.push_back(0.0f);
		H_total.push_back(0.0f);
		H_freq_total.push_back(0.0f);
		fftData.push_back(0.0f);
	}
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
	float value;
	for (int i = 0; i < fftSize * 2; i++)
	{
		fftData[i] = 0.0;
	}

	for (int i = 0; i < buffer.getNumSamples(); i++) {
		//value = outputBuffer.getSample(0, i);
		value = random.nextFloat() * 0.25f - 0.125f;
		value *= 10;

		fftData[i] = value;
		buffer.setSample(0, i, value);
		buffer.setSample(1, i, value);
	}

    updateParameters();

	forwardFFT.performRealOnlyForwardTransform(&fftData.at(0));
	/* Dot Product */
	for (int i = 0; i < fftSize * 2; i += 2)
	{
		float sr = fftData[i];
		float si = fftData[i + 1];
		float fr = H_total[i];
		float fi = H_total[i + 1];
		fftData[i] = sr * fr - si * fi;
		fftData[i + 1] = sr * fi + si * fr;
	}
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

void EQAudioProcessor::updateParameters()
{
    bool isUpdated = false;
    if (f[0] != tree.getRawParameterValue("lowpass1_f")->load()) {
        f[0] = tree.getRawParameterValue("lowpass1_f")->load();
        isUpdated = true;
    }
    if (Q[0] != tree.getRawParameterValue("lowpass1_q")->load()) {
        Q[0] = tree.getRawParameterValue("lowpass1_q")->load();
        isUpdated = true;
    }
    if (f[1] != tree.getRawParameterValue("highpass1_f")->load()) {
        f[1] = tree.getRawParameterValue("highpass1_f")->load();
        isUpdated = true;
    }
    if (Q[1] != tree.getRawParameterValue("highpass1_q")->load()) {
        Q[1] = tree.getRawParameterValue("highpass1_q")->load();
        isUpdated = true;
    }
    if (f[2] != tree.getRawParameterValue("notch1_f")->load()) {
        f[2] = tree.getRawParameterValue("notch1_f")->load();
        isUpdated = true;
    }
    if (Q[2] != tree.getRawParameterValue("notch1_q")->load()) {
        Q[2] = tree.getRawParameterValue("notch1_q")->load();
        isUpdated = true;
    }
    if (f[3] != tree.getRawParameterValue("notch2_f")->load()) {
        f[3] = tree.getRawParameterValue("notch2_f")->load();
        isUpdated = true;
    }
    if (Q[3] != tree.getRawParameterValue("notch2_q")->load()) {
        Q[3] = tree.getRawParameterValue("notch2_q")->load();
        isUpdated = true;
    }
    if (f[4] != tree.getRawParameterValue("lowshelf1_f")->load()) {
        f[4] = tree.getRawParameterValue("lowshelf1_f")->load();
        isUpdated = true;
    }
    if (Q[4] != tree.getRawParameterValue("lowshelf1_q")->load()) {
        Q[4] = tree.getRawParameterValue("lowshelf1_q")->load();
        isUpdated = true;
    }
    if (gain[0] != tree.getRawParameterValue("lowshelf1_gain")->load()) {
        gain[0] = tree.getRawParameterValue("lowshelf1_gain")->load();
        isUpdated = true;
    }
    if (f[5] != tree.getRawParameterValue("highshelf1_f")->load()) {
        f[5] = tree.getRawParameterValue("highshelf1_f")->load();
        isUpdated = true;
    }
    if (Q[5] != tree.getRawParameterValue("highshelf1_q")->load()) {
        Q[5] = tree.getRawParameterValue("highshelf1_q")->load();
        isUpdated = true;
    }
    if (gain[1] != tree.getRawParameterValue("highshelf1_gain")->load()) {
        gain[1] = tree.getRawParameterValue("highshelf1_gain")->load();
        isUpdated = true;
    }
    if (f[6] != tree.getRawParameterValue("peak1_f")->load()) {
        f[6] = tree.getRawParameterValue("peak1_f")->load();
        isUpdated = true;
    }
    if (Q[6] != tree.getRawParameterValue("peak1_q")->load()) {
        Q[6] = tree.getRawParameterValue("peak1_q")->load();
        isUpdated = true;
    }
    if (gain[2] != tree.getRawParameterValue("peak1_gain")->load()) {
        gain[2] = tree.getRawParameterValue("peak1_gain")->load();
        isUpdated = true;
    }
    if (f[7] != tree.getRawParameterValue("peak2_f")->load()) {
        f[7] = tree.getRawParameterValue("peak2_f")->load();
        isUpdated = true;
    }
    if (Q[7] != tree.getRawParameterValue("peak2_q")->load()) {
        Q[7] = tree.getRawParameterValue("peak2_q")->load();
        isUpdated = true;
    }
    if (gain[3] != tree.getRawParameterValue("peak2_gain")->load()) {
        gain[3] = tree.getRawParameterValue("peak2_gain")->load();
        isUpdated = true;
    }

    if (isUpdated)
        genFilter();
}

void EQAudioProcessor::genFilter()
{
	int order = qToOrder(Q.at(0)) + qToOrder(Q.at(1));
	/* Reset overlap size */
	delete[] overlap;
	overlap = new float[order] { 0 };

	genAllPass();
	genLowPass();
	genHighPass();
}

void EQAudioProcessor::genAllPass()
{
	int order = qToOrder(2.0f);
	/* Construct impulse response of All Pass Filter (FIR) */
	h[0] = 1.0f;
	for (int i = 1; i < order; i++)
		h[i] = 0.0f;

	/* FFT */
	for (int i = 0; i < fftSize * 2; i++)
	{
		H_total[i] = (i < order) ? h[i] : 0;
		H_freq_total[i] = (i < order) ? h[i] : 0;
	}

	forwardFFT.performRealOnlyForwardTransform(&H_total.at(0));
	forwardFFT.performFrequencyOnlyForwardTransform(&H_freq_total.at(0));

}
void EQAudioProcessor::genLowPass()
{
	int order = qToOrder(Q.at(0));
	/* Construct impulse response of Low Pass Filter (FIR) */
	auto fc = f.at(0) / getSampleRate();
	float impulse_response_sum = 0.0f;

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

	for (int i = 0; i < fftSize * 2; i += 2)
	{
		float r1 = H[i];
		float i1 = H[i + 1];
		float r2 = H_total[i];
		float i2 = H_total[i + 1];
		H_total[i] = r1 * r2 - i1 * i2;
		H_total[i + 1] = r1 * i2 + i1 * r2;
	}
	juce::FloatVectorOperations::multiply(&H_freq_total.at(0), &H_freq_total.at(0), &H_freq.at(0), fftSize * 2);
}
void EQAudioProcessor::genHighPass()
{
	for (int i = 0; i < 560; i++)
	{
		h[i] = 0;
	}
	int order = qToOrder(Q.at(1));
	/* Construct impulse response of Low Pass Filter (FIR) first */
	auto fc = f.at(1) / getSampleRate();
	float impulse_response_sum = 0.0f;

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

	for (int i = 0; i < fftSize * 2; i += 2)
	{
		float r1 = H[i];
		float i1 = H[i + 1];
		float r2 = H_total[i];
		float i2 = H_total[i + 1];
		H_total[i] = r1 * r2 - i1 * i2;
		H_total[i + 1] = r1 * i2 + i1 * r2;
	}
	juce::FloatVectorOperations::multiply(&H_freq_total.at(0), &H_freq_total.at(0), &H_freq.at(0), fftSize * 2);

}

int EQAudioProcessor::qToOrder(float q)
{
	auto map = [](float q)
	{
		return juce::jmap(q,
			0.1f, 2.0f,
			15.f, 255.f);
	};

	return ((int)map(q)%2==1) ? (int)map(q) : (int)map(q)-1;
}