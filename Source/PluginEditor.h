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

private:
    std::unique_ptr<Slider> gain, dryWet, tone;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment, dryWetAttachment, toneAttachment;
    Label gainLabel, dryWetLabel, toneLabel;
    
    std::unique_ptr<ComboBox> distortionSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> comboboxAttach;
    
    std::unique_ptr<TextButton> convolutionButton;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> convButtonAttach;
    
    Distors_PrototypesAudioProcessor& audioProcessor;
    
    KnobLookAndFeel knobLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distors_PrototypesAudioProcessorEditor)
};
