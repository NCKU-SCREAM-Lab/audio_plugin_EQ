#include "PluginProcessor.h"
#include "PluginEditor.h"

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
            "test1_f", "test1_F",
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "test1_q", "test1_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "test2_f", "test2_F",
            juce::NormalisableRange<float>(0.0f, 20000.0f, 1000.0f), 15000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "test2_q", "test2_Q",
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.1f), 1.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "test2_gain", "test_Gain",
            juce::NormalisableRange<float>(1.0f, 10.0f, 0.5f), 2.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int){ return juce::String(value); },
            [](juce::String text){ return text.getFloatValue(); }
        )
    })
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
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto *channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
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