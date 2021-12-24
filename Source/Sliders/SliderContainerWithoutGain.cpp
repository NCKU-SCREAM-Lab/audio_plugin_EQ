/*
  ==============================================================================

    SliderContainerWithoutGain.cpp
    Created: 24 Dec 2021 10:12:21pm
    Author:  robert0401, abao1005

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderContainerWithoutGain.h"

SliderContainerWithoutGain::SliderContainerWithoutGain(EQAudioProcessor &p, juce::String filterName)
    : audioProcessor(p),
      fSlider(p, "f", filterName),
      QSlider(p, "q", filterName)

{    
    for (auto &slider : sliders) {
        addAndMakeVisible(slider);
    }
}

SliderContainerWithoutGain::~SliderContainerWithoutGain()
{

}

void SliderContainerWithoutGain::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SliderContainerWithoutGain::resized()
{
    juce::FlexBox flexBox;
    for (auto &slider : sliders) {
        juce::FlexItem item {*slider};
        flexBox.items.add(item.withFlex(1.0));
    }
    flexBox.performLayout(getLocalBounds());
    for (auto &slider : sliders) {
        slider->setBounds(slider->getBounds().reduced(5));
    }
}
