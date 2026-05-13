#pragma once

#include <JuceHeader.h>

class VoxMasterAudioProcessor;

class VoxMasterAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit VoxMasterAudioProcessorEditor(VoxMasterAudioProcessor& processor);
    ~VoxMasterAudioProcessorEditor() override = default;

    void paint(juce::Graphics& graphics) override;
    void resized() override;

private:
    VoxMasterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoxMasterAudioProcessorEditor)
};
