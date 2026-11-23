#include "PluginEditor.h"

VelvetBusCompAudioProcessorEditor::VelvetBusCompAudioProcessorEditor(VelvetBusCompAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(650, 390);
    title.setText("VELVETBUS COMP", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(27.0f, juce::Font::bold)));
    title.setColour(juce::Label::textColourId, juce::Colour(0xfff2e8d5));
    title.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(title);

    subtitle.setText("TUBE  •  TRANSFORMER  •  CONSOLE COLOUR", juce::dontSendNotification);
    subtitle.setFont(juce::Font(juce::FontOptions(11.0f)));
    subtitle.setColour(juce::Label::textColourId, juce::Colour(0xffc6a978));
    subtitle.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subtitle);

    setupSlider(input, "INPUT");
    setupSlider(attack, "ATTACK");
    setupSlider(release, "RELEASE");
    setupSlider(output, "OUTPUT");
    setupSlider(ratio, "RATIO");
    setupSlider(colour, "ANALOG COLOR");

    auto& state = processor.parameters;
    inputAttach = std::make_unique<Attachment>(state, "INPUT", input);
    attackAttach = std::make_unique<Attachment>(state, "ATTACK", attack);
    releaseAttach = std::make_unique<Attachment>(state, "RELEASE", release);
    outputAttach = std::make_unique<Attachment>(state, "OUTPUT", output);
    ratioAttach = std::make_unique<Attachment>(state, "RATIO", ratio);
    colourAttach = std::make_unique<Attachment>(state, "COLOR", colour);
}

void VelvetBusCompAudioProcessorEditor::setupSlider(juce::Slider& s, const juce::String& name)
{
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 78, 22);
    s.setName(name);
    s.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffc6a05b));
    s.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff4c4a46));
    s.setColour(juce::Slider::thumbColourId, juce::Colour(0xfff4e7cf));
    s.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xfff2e8d5));
    s.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff242424));
    s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff5d5548));
    addAndMakeVisible(s);
}

void VelvetBusCompAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff171716));
    g.setColour(juce::Colour(0xff2b2925));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(12.0f), 14.0f);
    g.setColour(juce::Colour(0xff8e7043));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(12.0f), 14.0f, 1.5f);
    g.setColour(juce::Colour(0xff39352e));
    g.drawLine(35.0f, 87.0f, 615.0f, 87.0f, 1.0f);
    g.setColour(juce::Colour(0xffbda77f));
    g.setFont(11.0f);
    g.drawFittedText("GENERAL PURPOSE ANALOG-STYLE DYNAMICS", 25, 350, 600, 18,
                      juce::Justification::centred, 1);
}

void VelvetBusCompAudioProcessorEditor::resized()
{
    title.setBounds(25, 20, 600, 38);
    subtitle.setBounds(25, 58, 600, 20);
    const int top = 104;
    const int w = 190;
    const int h = 112;
    input.setBounds(35, top, w, h);
    attack.setBounds(230, top, w, h);
    release.setBounds(425, top, w, h);
    output.setBounds(35, 225, w, h);
    ratio.setBounds(230, 225, w, h);
    colour.setBounds(425, 225, w, h);
}
