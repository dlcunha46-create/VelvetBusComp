#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class VelvetBusCompAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit VelvetBusCompAudioProcessorEditor(VelvetBusCompAudioProcessor&);
    ~VelvetBusCompAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    VelvetBusCompAudioProcessor& processor;
    juce::Slider input, attack, release, output, ratio, colour;
    juce::Label title, subtitle;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> inputAttach, attackAttach, releaseAttach, outputAttach, ratioAttach, colourAttach;

    void setupSlider(juce::Slider&, const juce::String&);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelvetBusCompAudioProcessorEditor)
};
