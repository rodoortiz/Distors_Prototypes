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
    gain = std::make_unique<Slider>();
    gain->setSliderStyle(Slider::RotaryVerticalDrag);
    gain->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(gain.get());
    
    knob1Attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB1", *gain);
    
    Label1.setText("Gain", dontSendNotification);
    Label1.attachToComponent(gain.get(), false);
    Label1.setJustificationType(Justification::centred);
    addAndMakeVisible(Label1);
    
    dryWet = std::make_unique<Slider>();
    dryWet->setSliderStyle(Slider::RotaryVerticalDrag);
    dryWet->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(dryWet.get());
    
    knob2Attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "KNOB2", *dryWet);
    
    Label2.setText("DryWet", dontSendNotification);
    Label2.attachToComponent(dryWet.get(), false);
    Label2.setJustificationType(Justification::centred);
    
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
    gain->setBoundsRelative(0.1f, 0.1f, 0.4f, 0.4f);
    dryWet->setBoundsRelative(0.4f, 0.1f, 0.4f, 0.4f);

}
