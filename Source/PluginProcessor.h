/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Distortions.h"



//==============================================================================
/**
*/
class Distors_PrototypesAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Distors_PrototypesAudioProcessor();
    ~Distors_PrototypesAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState apvts;
    AudioProcessorValueTreeState::ParameterLayout parameterLayout();

private:
    
    Distortions distortions;
    dsp::Convolution convolution_1;
    dsp::Convolution convolution_2;
    dsp::Convolution convolution_3;
    dsp::StateVariableTPTFilter<float> toneFilter;
    
    float dryWetValue;
    
    // Comentario de prueba de un fork
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distors_PrototypesAudioProcessor)
};
