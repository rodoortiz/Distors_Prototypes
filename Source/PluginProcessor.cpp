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
    
    params.push_back (std::make_unique<AudioParameterFloat>(ParameterID { "GAIN", 1 },
                                                            "Gain",
                                                            NormalisableRange<float> (1.0f, 100.0f, 0.01f),
                                                            1.0f));
    
    params.push_back (std::make_unique<AudioParameterFloat>(ParameterID { "DRYWET", 1 },
                                                            "DryWet",
                                                            NormalisableRange<float> (0.0f, 1.0f, 0.01f),
                                                            0.5f));
    
    params.push_back (std::make_unique<AudioParameterFloat>(ParameterID { "TONE", 1 },
                                                            "Tone",
                                                            NormalisableRange<float> (20.0f, 20000.0f, 1.0f),
                                                            20000.0f));
    
    int numOfDistortions = 14;
    params.push_back (std::make_unique<AudioParameterInt>(ParameterID { "DISTOR_SELECT", 1 },
                                                          "Distortion Selector",
                                                          1,
                                                          numOfDistortions,
                                                          1));
    
    int numOfConvolutions = 3;
    params.push_back (std::make_unique<AudioParameterInt>(ParameterID { "CONV_SELECT", 1 },
                                                          "Convolution Selector",
                                                          1,
                                                          numOfConvolutions,
                                                          1));
    
    params.push_back (std::make_unique<AudioParameterBool>(ParameterID { "CONVOLUTION", 1 },
                                                           "Convolution",
                                                           false));
    
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

void Distors_PrototypesAudioProcessor::setCurrentProgram ([[maybe_unused]] int index)
{
}

const juce::String Distors_PrototypesAudioProcessor::getProgramName ([[maybe_unused]] int index)
{
    return {};
}

void Distors_PrototypesAudioProcessor::changeProgramName ([[maybe_unused]] int index, [[maybe_unused]] const juce::String& newName)
{
}

//==============================================================================
void Distors_PrototypesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.sampleRate = sampleRate;
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());
    
    convolution_1.prepare (spec);
    convolution_1.loadImpulseResponse (BinaryData::metalOne_wav,
                                       BinaryData::metalOne_wavSize,
                                       juce::dsp::Convolution::Stereo::yes,
                                       juce::dsp::Convolution::Trim::yes,
                                       0,
                                       juce::dsp::Convolution::Normalise::yes);

    convolution_2.prepare (spec);
    convolution_2.loadImpulseResponse (BinaryData::Fredman_Straight_wav,
                                       BinaryData::Fredman_Straight_wavSize,
                                       juce::dsp::Convolution::Stereo::yes,
                                       juce::dsp::Convolution::Trim::yes,
                                       0,
                                       juce::dsp::Convolution::Normalise::yes);
    
    convolution_3.prepare (spec);
    convolution_3.loadImpulseResponse (BinaryData::GuitarHack_JJ_CENTRE45_0_wav,
                                       BinaryData::GuitarHack_JJ_CENTRE45_0_wavSize,
                                       juce::dsp::Convolution::Stereo::yes,
                                       juce::dsp::Convolution::Trim::yes,
                                       0,
                                       juce::dsp::Convolution::Normalise::yes);
    
    toneFilter.prepare (spec);
    toneFilter.setType (dsp::StateVariableTPTFilterType::lowpass);
    
    dryWet.prepare (spec);
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

void Distors_PrototypesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto audioBlock = juce::dsp::AudioBlock<float> (buffer);
    dryWet.pushDrySamples (audioBlock);
    
    auto distortionSelected = static_cast<int> (apvts.getRawParameterValue ("DISTOR_SELECT")->load());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            float sample = buffer.getWritePointer(channel)[n];
            float sampleProcessed;
            auto gainValue = apvts.getRawParameterValue("GAIN")->load();
            
            switch (distortionSelected)
            {
                case 1:
                    sampleProcessed = distortions.arcTanDistortion (sample, gainValue);
                    break;
                case 2:
                    sampleProcessed = distortions.softClipper (sample, gainValue);
                    break;
                case 3:
                    sampleProcessed = distortions.sigmoid (sample, gainValue);
                    break;
                case 4:
                    sampleProcessed = distortions.hyperbolicTangent (sample, gainValue);
                    break;
                case 5:
                    sampleProcessed = distortions.diodeClipping (sample, gainValue);
                    break;
                case 6:
                    sampleProcessed = distortions.fuzzExponential (sample, gainValue);
                    break;
                case 7:
                    sampleProcessed = distortions.pieceWiseOverdrive (sample, gainValue);
                    break;
                case 8:
                    sampleProcessed = distortions.tube (sample, gainValue);
                    break;
                case 9:
                    sampleProcessed = distortions.arraya (sample, gainValue);
                    break;
                case 10:
                    sampleProcessed = distortions.gallo (sample, gainValue);
                    break;
                case 11:
                    sampleProcessed = distortions.doubleSoftClipper (sample, gainValue);
                    break;
                case 12:
                    sampleProcessed = distortions.crush (sample, gainValue);
                    break;
                case 13:
                    sampleProcessed = distortions.tuboid (sample, gainValue);
                    break;
                case 14:
                    sampleProcessed = distortions.pakarinen_Yeh (sample, gainValue);
                    break;
                    
                default:
                    sampleProcessed = sample;
                    break;
            }

            buffer.getWritePointer(channel)[n] = sampleProcessed;
        }
    }
    
    auto convolutionState = static_cast<bool>(apvts.getRawParameterValue ("CONVOLUTION")->load());
    
    if (convolutionState)
    {
        auto block = dsp::AudioBlock<float> (buffer);
        auto context = dsp::ProcessContextReplacing<float> (block);
        
        auto convolutionSelected = static_cast<int>(apvts.getRawParameterValue ("CONV_SELECT")->load());
        
        switch (convolutionSelected)
        {
            case 1:
                convolution_1.process (context);
                break;
            case 2:
                convolution_2.process (context);
                break;
            case 3:
                convolution_3.process (context);
                break;
                
            default:
                break;
        }
        
        buffer.applyGain (0, buffer.getNumSamples(), 5.0f);
    }
    
    toneFilter.setCutoffFrequency (apvts.getRawParameterValue ("TONE")->load());
    
    auto context = dsp::ProcessContextReplacing<float> (audioBlock);
    toneFilter.process (context);
    
    dryWet.setWetMixProportion (apvts.getRawParameterValue ("DRYWET")->load());
    dryWet.mixWetSamples (audioBlock);
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
    juce::MemoryOutputStream stream(destData, false);
    apvts.state.writeToStream(stream);
}

void Distors_PrototypesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData (data, size_t (sizeInBytes));
    if (tree.isValid()) {
        apvts.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Distors_PrototypesAudioProcessor();
}
