#include "PluginProcessor.h"
#include "PluginEditor.h"

VoxMasterAudioProcessor::VoxMasterAudioProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout VoxMasterAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "inputGain", 1 }, "Input Gain", juce::NormalisableRange<float> { -24.0f, 24.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "outputGain", 1 }, "Output Gain", juce::NormalisableRange<float> { -24.0f, 24.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID { "aiAutoApplyEnabled", 1 }, "AI Auto Apply", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "aiApplyAmount", 1 }, "AI Apply Amount", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID { "localFeedbackEnabled", 1 }, "Local Feedback", false));

    return { params.begin(), params.end() };
}

void VoxMasterAudioProcessor::prepareToPlay(double, int) {}
void VoxMasterAudioProcessor::releaseResources() {}

bool VoxMasterAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainOutput = layouts.getMainOutputChannelSet();
    const auto& mainInput = layouts.getMainInputChannelSet();

    return mainOutput == mainInput && (mainOutput == juce::AudioChannelSet::mono() || mainOutput == juce::AudioChannelSet::stereo());
}

void VoxMasterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    for (auto channel = getTotalNumInputChannels(); channel < getTotalNumOutputChannels(); ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* VoxMasterAudioProcessor::createEditor()
{
    return new VoxMasterAudioProcessorEditor(*this);
}

bool VoxMasterAudioProcessor::hasEditor() const { return true; }
const juce::String VoxMasterAudioProcessor::getName() const { return JucePlugin_Name; }
bool VoxMasterAudioProcessor::acceptsMidi() const { return false; }
bool VoxMasterAudioProcessor::producesMidi() const { return false; }
bool VoxMasterAudioProcessor::isMidiEffect() const { return false; }
double VoxMasterAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int VoxMasterAudioProcessor::getNumPrograms() { return 1; }
int VoxMasterAudioProcessor::getCurrentProgram() { return 0; }
void VoxMasterAudioProcessor::setCurrentProgram(int) {}
const juce::String VoxMasterAudioProcessor::getProgramName(int) { return {}; }
void VoxMasterAudioProcessor::changeProgramName(int, const juce::String&) {}
void VoxMasterAudioProcessor::getStateInformation(juce::MemoryBlock&) {}
void VoxMasterAudioProcessor::setStateInformation(const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoxMasterAudioProcessor();
}
