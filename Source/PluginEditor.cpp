#include "PluginEditor.h"
#include "PluginProcessor.h"

VoxMasterAudioProcessorEditor::VoxMasterAudioProcessorEditor(VoxMasterAudioProcessor& processor)
    : AudioProcessorEditor(&processor), audioProcessor(processor)
{
    juce::ignoreUnused(audioProcessor);
    setSize(960, 600);
}

void VoxMasterAudioProcessorEditor::paint(juce::Graphics& graphics)
{
    graphics.fillAll(juce::Colour::fromRGB(13, 16, 24));
    graphics.setColour(juce::Colours::white);
    graphics.setFont(juce::FontOptions(28.0f, juce::Font::bold));
    graphics.drawFittedText("VoxMaster", getLocalBounds().reduced(24), juce::Justification::centredTop, 1);

    graphics.setFont(juce::FontOptions(15.0f));
    graphics.setColour(juce::Colour::fromRGB(180, 190, 210));
    graphics.drawFittedText("Phase 1 scaffold: JUCE + CMake + ONNX Runtime-ready architecture", getLocalBounds().reduced(24).withTrimmedTop(56), juce::Justification::centredTop, 2);
}

void VoxMasterAudioProcessorEditor::resized() {}
