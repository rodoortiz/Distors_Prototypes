/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"

//==============================================================================
/**
*/
class Distors_PrototypesAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Distors_PrototypesAudioProcessorEditor (Distors_PrototypesAudioProcessor&);
    ~Distors_PrototypesAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setSlider (Slider* slider);
    void setLabel (Label* label, String labelText);

private:
    Distors_PrototypesAudioProcessor& audioProcessor;

    std::unique_ptr<Slider> gain;
    std::unique_ptr<Slider> dryWet;
    std::unique_ptr<Slider> tone;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    
    Label gainLabel;
    Label dryWetLabel;
    Label toneLabel;
    
    std::unique_ptr<ComboBox> distortionSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttach;
    
    std::unique_ptr<TextButton> convolutionButton;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> convButtonAttach;
    
    std::unique_ptr<ComboBox> convolutionSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> convComboBoxAttach;
    
    KnobLookAndFeel knobLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distors_PrototypesAudioProcessorEditor)
};
