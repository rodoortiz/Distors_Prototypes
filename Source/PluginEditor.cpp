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
    //KNOB 1 Gain
    gain = std::make_unique<Slider>();
    gain->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    gain->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    gain->setLookAndFeel(&knobLookAndFeel);
    addAndMakeVisible(gain.get());
    
    gainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB1", *gain);
    
    gainLabel.setText("Gain", dontSendNotification);
    gainLabel.attachToComponent(gain.get(), false);
    gainLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(gainLabel);
    
    //KNOB 2 DryWet
    dryWet = std::make_unique<Slider>();
    dryWet->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    dryWet->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(dryWet.get());
    
    dryWetAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB2", *dryWet);
    
    dryWetLabel.setText("DryWet", dontSendNotification);
    dryWetLabel.attachToComponent(dryWet.get(), false);
    dryWetLabel.setJustificationType(Justification::centred);
    
    //KNOB 3 Tone
    tone = std::make_unique<Slider>();
    tone->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    tone->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    tone->setLookAndFeel(&knobLookAndFeel);
    addAndMakeVisible(tone.get());
    
    toneAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB3", *tone);
    
    toneLabel.setText("Tone", dontSendNotification);
    toneLabel.attachToComponent(tone.get(), false);
    toneLabel.setJustificationType(Justification::centred);
    
    //COMBOBOX DISTORS
    distortionSelector = std::make_unique<ComboBox>();
    distortionSelector->setTextWhenNothingSelected("Select Distortion");
    distortionSelector->addItem("arcTanDistortion", 1);
    distortionSelector->addItem("softClipper", 2);
    distortionSelector->addItem("sigmoid", 3);
    distortionSelector->addItem("hyperbolicTangent", 4);
    distortionSelector->addItem("diodeClipping", 5);
    distortionSelector->addItem("fuzzExponential", 6);
    distortionSelector->addItem("pieceWiseOverdirve", 7);
    distortionSelector->addItem("tube", 8);
    distortionSelector->addItem("arraya", 9);
    distortionSelector->addItem("gallo", 10);
    distortionSelector->addItem("doubleSoftClipper", 11);
    distortionSelector->addItem("crush", 12);
    
    addAndMakeVisible(distortionSelector.get());
    
    comboBoxAttach = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "DISTOR_SELECT", *distortionSelector);
    
    //BUTTON CONVOLUTION
    convolutionButton = std::make_unique<TextButton>("Convolution");
    convolutionButton->setClickingTogglesState(true);
    addAndMakeVisible(convolutionButton.get());
    
    convButtonAttach = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, "CONVOLUTION", *convolutionButton);
    
    //COMBOBOX CONVOLUTION
    
    convolutionSelector = std::make_unique<ComboBox>();
    convolutionSelector->setTextWhenNothingSelected("Select Distortion");
    convolutionSelector->addItem("MetalOne", 1);
    convolutionSelector->addItem("Fredman", 2);
    convolutionSelector->addItem("GuitarHack", 3);

    addAndMakeVisible(convolutionSelector.get());
    
    convComboBoxAttach = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "CONV_SELECT", *convolutionSelector);
    
    setSize (400, 300);
}

Distors_PrototypesAudioProcessorEditor::~Distors_PrototypesAudioProcessorEditor()
{
    gain->setLookAndFeel(nullptr);
    tone->setLookAndFeel(nullptr);
}

//==============================================================================
void Distors_PrototypesAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void Distors_PrototypesAudioProcessorEditor::resized()
{
    distortionSelector->setBoundsRelative(0.05f, 0.1f, 0.4f, 0.1f);
    convolutionSelector->setBoundsRelative(0.5f, 0.1f, 0.2f, 0.1f);
    convolutionButton->setBoundsRelative(0.75f, 0.1f, 0.2f, 0.1f);
    gain->setBoundsRelative(0.1f, 0.3f, 0.4f, 0.4f);
    dryWet->setBoundsRelative(0.5f, 0.3f, 0.4f, 0.4f);
    tone->setBoundsRelative(0.3f, 0.6, 0.4f, 0.4f);

}
