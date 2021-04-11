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
    
    knob1Attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB1", *gain);
    
    Label1.setText("Gain", dontSendNotification);
    Label1.attachToComponent(gain.get(), false);
    Label1.setJustificationType(Justification::centred);
    addAndMakeVisible(Label1);
    
    //KNOB 2
    dryWet = std::make_unique<Slider>();
    dryWet->setSliderStyle(Slider::RotaryVerticalDrag);
    dryWet->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(dryWet.get());
    
    knob2Attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB2", *dryWet);
    
    Label2.setText("DryWet", dontSendNotification);
    Label2.attachToComponent(dryWet.get(), false);
    Label2.setJustificationType(Justification::centred);
    
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
    dryWet->setBoundsRelative(0.4f, 0.3f, 0.4f, 0.4f);

}
