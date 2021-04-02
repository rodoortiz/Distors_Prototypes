/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    std::unique_ptr<Slider> gain, dryWet;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> knob1Attachment, knob2Attachment;
    Label Label1, Label2;
    
    Distors_PrototypesAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distors_PrototypesAudioProcessorEditor)
};
