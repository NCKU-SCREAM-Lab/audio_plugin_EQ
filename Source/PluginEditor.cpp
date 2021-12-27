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
    setSize(600, 600);

    for (auto &subcomponent : subcomponents) {
        addAndMakeVisible(subcomponent);
    }
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
    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::column;
    // flexBox.items.add(juce::FlexItem(spectrum).withFlex(3.0f));
    flexBox.items.add(juce::FlexItem(lowpass1).withFlex(1.0f));
    flexBox.items.add(juce::FlexItem(highpass1).withFlex(1.0f));
    flexBox.items.add(juce::FlexItem(notch1).withFlex(1.0f));
    flexBox.items.add(juce::FlexItem(notch2).withFlex(1.0f));
    flexBox.items.add(juce::FlexItem(lowshelf1).withFlex(1.0f));
    flexBox.items.add(juce::FlexItem(highshelf1).withFlex(1.0f));
    flexBox.items.add(juce::FlexItem(peak1).withFlex(1.0f));
    flexBox.items.add(juce::FlexItem(peak2).withFlex(1.0f));
    flexBox.performLayout(area.reduced(10));
    for (auto &subcomponent : subcomponents) {
        subcomponent->setBounds(subcomponent->getBounds().reduced(3));
    }
}
