/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Distors_PrototypesAudioProcessorEditor::Distors_PrototypesAudioProcessorEditor (Distors_PrototypesAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //KNOB 1
    gain = std::make_unique<Slider>();
    gain->setSliderStyle(Slider::RotaryVerticalDrag);
    gain->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(gain.get());
    
    gainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB1", *gain);
    
    gainLabel.setText("Gain", dontSendNotification);
    gainLabel.attachToComponent(gain.get(), false);
    gainLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(gainLabel);
    
    //KNOB 2
    dryWet = std::make_unique<Slider>();
    dryWet->setSliderStyle(Slider::RotaryVerticalDrag);
    dryWet->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(dryWet.get());
    
    dryWetAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB2", *dryWet);
    
    dryWetLabel.setText("DryWet", dontSendNotification);
    dryWetLabel.attachToComponent(dryWet.get(), false);
    dryWetLabel.setJustificationType(Justification::centred);
    
    //KNOB 3
    tone = std::make_unique<Slider>();
    tone->setSliderStyle(Slider::RotaryVerticalDrag);
    tone->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(tone.get());
    
    toneAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB3", *tone);
    
    toneLabel.setText("Tone", dontSendNotification);
    toneLabel.attachToComponent(tone.get(), false);
    toneLabel.setJustificationType(Justification::centred);
    
    //COMBOBOX
    distortionSelector = std::make_unique<ComboBox>();
    distortionSelector->setTextWhenNothingSelected("Select Distortion");
    distortionSelector->addItem("arcTanDistortion", 1);
    distortionSelector->addItem(" softClipper", 2);
    distortionSelector->addItem("sigmoid", 3);
    distortionSelector->addItem("hyperbolicTangent", 4);
    distortionSelector->addItem("diodeClipping", 5);
    distortionSelector->addItem("fuzzExponential", 6);
    distortionSelector->addItem("pieceWiseOverdirve", 7);

    addAndMakeVisible(distortionSelector.get());
    
    comboboxAttach = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "DISTOR_SELECT", *distortionSelector);
    
    //BUTTON
    convolutionButton = std::make_unique<TextButton>("Convolution");
    convolutionButton->setClickingTogglesState(true);
    addAndMakeVisible(convolutionButton.get());
    
    convButtonAttach = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, "CONVOLUTION", *convolutionButton);
    
    setSize (400, 300);
}

Distors_PrototypesAudioProcessorEditor::~Distors_PrototypesAudioProcessorEditor()
{
}

//==============================================================================
void Distors_PrototypesAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void Distors_PrototypesAudioProcessorEditor::resized()
{
    distortionSelector->setBoundsRelative(0.1f, 0.1f, 0.5f, 0.1f);
    convolutionButton->setBoundsRelative(0.7f, 0.1f, 0.2f, 0.1f);
    gain->setBoundsRelative(0.1f, 0.3f, 0.4f, 0.4f);
    dryWet->setBoundsRelative(0.5f, 0.3f, 0.4f, 0.4f);
    tone->setBoundsRelative(0.3f, 0.6, 0.4f, 0.4f);

}
