/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Distors_PrototypesAudioProcessor::Distors_PrototypesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", parameterLayout())
#endif
{
}

Distors_PrototypesAudioProcessor::~Distors_PrototypesAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout Distors_PrototypesAudioProcessor::parameterLayout() {
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<AudioParameterFloat>("KNOB1", "Knob1", NormalisableRange<float> (1.0f, 100.0f, 0.01), 10.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("KNOB2", "Knob2", NormalisableRange<float> (0.0f, 1.0f, 0.01), 0.5f));
    
    return {params.begin(), params.end()};
    
}

//==============================================================================
const juce::String Distors_PrototypesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Distors_PrototypesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Distors_PrototypesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Distors_PrototypesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Distors_PrototypesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Distors_PrototypesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Distors_PrototypesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Distors_PrototypesAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Distors_PrototypesAudioProcessor::getProgramName (int index)
{
    return {};
}

void Distors_PrototypesAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Distors_PrototypesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Distors_PrototypesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Distors_PrototypesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Distors_PrototypesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int n = 0; n < buffer.getNumSamples(); ++n) {
            float sample = buffer.getWritePointer(channel)[n];

            float sampleProcessed = distortions.arcTanDistortion(sample, apvts.getRawParameterValue("KNOB1")->load());
            
//            float sampleProcessed = distortions.softClipper(sample, apvts.getRawParameterValue("KNOB1")->load());
            
            dryWetValue = apvts.getRawParameterValue("KNOB2")->load();
            float output = ((1.0f - dryWetValue) * sample) + (dryWetValue * sampleProcessed);

            buffer.getWritePointer(channel)[n] = output;
        }
    }
}

//==============================================================================
bool Distors_PrototypesAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Distors_PrototypesAudioProcessor::createEditor()
{
    return new Distors_PrototypesAudioProcessorEditor (*this);
}

//==============================================================================
void Distors_PrototypesAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Distors_PrototypesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Distors_PrototypesAudioProcessor();
}
