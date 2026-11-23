#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    constexpr float thresholdDb = -18.0f;
    constexpr float pi = 3.14159265358979323846f;

    float dbToGain(float db) { return std::pow(10.0f, db / 20.0f); }

    float tubeColour(float x, float amount)
    {
        if (amount <= 0.0001f) return x;
        const float drive = 1.0f + amount * 5.0f;
        const float normaliser = std::tanh(drive);
        const float shaped = std::tanh(x * drive) / normaliser;
        return x + (shaped - x) * amount;
    }

    float transformerColour(float x, float amount)
    {
        if (amount <= 0.0001f) return x;
        // Gentle asymmetric magnetic-style saturation; deliberately subtle.
        const float drive = 1.0f + amount * 3.2f;
        const float y = std::tanh((x + 0.035f * x * x) * drive) / std::tanh(drive);
        return x + (y - x) * amount * 0.8f;
    }

    float consoleColour(float x, float amount)
    {
        if (amount <= 0.0001f) return x;
        // Clean console-inspired soft-knee density, not an emulation of a specific circuit.
        const float y = x / (1.0f + 0.16f * amount * std::abs(x));
        return x + (y - x) * amount * 0.65f;
    }
}

VelvetBusCompAudioProcessor::VelvetBusCompAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout VelvetBusCompAudioProcessor::createParameterLayout()
{
    using namespace juce;
    std::vector<std::unique_ptr<RangedAudioParameter>> p;
    p.push_back(std::make_unique<AudioParameterFloat>("INPUT", "Input", NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, " dB"));
    p.push_back(std::make_unique<AudioParameterFloat>("ATTACK", "Attack", NormalisableRange<float>(0.1f, 100.0f, 0.1f, 0.35f), 15.0f, " ms"));
    p.push_back(std::make_unique<AudioParameterFloat>("RELEASE", "Release", NormalisableRange<float>(10.0f, 1000.0f, 1.0f, 0.35f), 120.0f, " ms"));
    p.push_back(std::make_unique<AudioParameterFloat>("OUTPUT", "Output", NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, " dB"));
    p.push_back(std::make_unique<AudioParameterFloat>("RATIO", "Ratio", NormalisableRange<float>(1.0f, 10.0f, 0.1f), 4.0f, ":1"));
    p.push_back(std::make_unique<AudioParameterFloat>("COLOR", "Analog Color", NormalisableRange<float>(0.0f, 100.0f, 0.1f), 35.0f, "%"));
    return { p.begin(), p.end() };
}

void VelvetBusCompAudioProcessor::prepareToPlay(double sampleRate, int)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    envelope = 0.0f;
}

void VelvetBusCompAudioProcessor::releaseResources() {}

bool VelvetBusCompAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto in = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();
    return in == out && (in == juce::AudioChannelSet::mono() || in == juce::AudioChannelSet::stereo());
}

void VelvetBusCompAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int channels = getTotalNumInputChannels();
    const int samples = buffer.getNumSamples();

    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, samples);

    const float inputDb = parameters.getRawParameterValue("INPUT")->load();
    const float attackMs = parameters.getRawParameterValue("ATTACK")->load();
    const float releaseMs = parameters.getRawParameterValue("RELEASE")->load();
    const float outputDb = parameters.getRawParameterValue("OUTPUT")->load();
    const float ratio = parameters.getRawParameterValue("RATIO")->load();
    const float colour = parameters.getRawParameterValue("COLOR")->load() * 0.01f;

    const float inputGain = dbToGain(inputDb);
    const float outputGain = dbToGain(outputDb);
    const float attackCoeff = std::exp(-1.0f / (0.001f * attackMs * static_cast<float>(currentSampleRate)));
    const float releaseCoeff = std::exp(-1.0f / (0.001f * releaseMs * static_cast<float>(currentSampleRate)));

    for (int i = 0; i < samples; ++i)
    {
        float detector = 0.0f;
        for (int ch = 0; ch < channels; ++ch)
            detector += std::abs(buffer.getSample(ch, i) * inputGain);
        detector /= static_cast<float>(juce::jmax(1, channels));

        const float coeff = detector > envelope ? attackCoeff : releaseCoeff;
        envelope = coeff * envelope + (1.0f - coeff) * detector;

        const float levelDb = 20.0f * std::log10(juce::jmax(envelope, 1.0e-8f));
        const float overDb = juce::jmax(0.0f, levelDb - thresholdDb);
        const float gainReductionDb = overDb * (1.0f / ratio - 1.0f);
        const float compGain = dbToGain(gainReductionDb);

        for (int ch = 0; ch < channels; ++ch)
        {
            float x = buffer.getSample(ch, i) * inputGain * compGain;
            x = tubeColour(x, colour * 0.42f);
            x = transformerColour(x, colour * 0.34f);
            x = consoleColour(x, colour * 0.24f);
            buffer.setSample(ch, i, x * outputGain);
        }
    }
}

juce::AudioProcessorEditor* VelvetBusCompAudioProcessor::createEditor()
{
    return new VelvetBusCompAudioProcessorEditor(*this);
}

void VelvetBusCompAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VelvetBusCompAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VelvetBusCompAudioProcessor();
}
