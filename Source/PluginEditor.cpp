#include "PluginProcessor.h"
#include "PluginEditor.h"

EQAudioProcessorEditor::EQAudioProcessorEditor(EQAudioProcessor &p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      spectrum(p),
      lowpass1(p, "Low Pass", 1),
      highpass1(p, "High Pass", 1),
      notch1(p, "Notch", 1),
      notch2(p, "Notch", 2),
      lowshelf1(p, "Low Shelf", 1),
      highshelf1(p, "High Shelf", 1),
      peak1(p, "Peak", 1),
      peak2(p, "Peak", 2)
{
    setSize(600, 750);

    for (auto &subcomponent : subcomponents) {
        addAndMakeVisible(subcomponent);
    }
	AudioProcessorEditor::setResizable(true, true);
	AudioProcessorEditor::setResizeLimits(600, 750, 1800, 2250);
}

EQAudioProcessorEditor::~EQAudioProcessorEditor()
{

}

void EQAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void EQAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    juce::FlexBox main, bandContainer1, bandContainer2;
    bandContainer1.flexDirection = juce::FlexBox::Direction::row;
    bandContainer1.items.add(juce::FlexItem(lowpass1).withFlex(1.0f));
    bandContainer1.items.add(juce::FlexItem(highpass1).withFlex(1.0f));
    bandContainer1.items.add(juce::FlexItem(notch1).withFlex(1.0f));
    bandContainer1.items.add(juce::FlexItem(notch2).withFlex(1.0f));
    bandContainer1.performLayout(area.reduced(10));
    bandContainer2.flexDirection = juce::FlexBox::Direction::row;
    bandContainer2.items.add(juce::FlexItem(lowshelf1).withFlex(1.0f));
    bandContainer2.items.add(juce::FlexItem(highshelf1).withFlex(1.0f));
    bandContainer2.items.add(juce::FlexItem(peak1).withFlex(1.0f));
    bandContainer2.items.add(juce::FlexItem(peak2).withFlex(1.0f));
    bandContainer2.performLayout(area.reduced(10));
    main.flexDirection = juce::FlexBox::Direction::column;
    main.items.add(juce::FlexItem(spectrum).withFlex(2.5f));
    main.items.add(juce::FlexItem(bandContainer1).withFlex(1.5f));
    main.items.add(juce::FlexItem(bandContainer2).withFlex(2.0f));
    main.performLayout(area.reduced(10));
    for (auto &subcomponent : subcomponents) {
        subcomponent->setBounds(subcomponent->getBounds().reduced(3));
    }
}
