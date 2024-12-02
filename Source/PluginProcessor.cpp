/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HYLC_Tape_Delay_V1AudioProcessor::HYLC_Tape_Delay_V1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

HYLC_Tape_Delay_V1AudioProcessor::~HYLC_Tape_Delay_V1AudioProcessor()
{
}

//==============================================================================
const juce::String HYLC_Tape_Delay_V1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HYLC_Tape_Delay_V1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HYLC_Tape_Delay_V1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HYLC_Tape_Delay_V1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HYLC_Tape_Delay_V1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HYLC_Tape_Delay_V1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HYLC_Tape_Delay_V1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void HYLC_Tape_Delay_V1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HYLC_Tape_Delay_V1AudioProcessor::getProgramName (int index)
{
    return {};
}

void HYLC_Tape_Delay_V1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HYLC_Tape_Delay_V1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    bufferSize = static_cast<int>(sampleRate * 5.0); // 5 seconds of audio
    circularBuffer.resize(bufferSize, 0.0f);
    writePosition = 0;
    playbackPosition = 0;
}

void HYLC_Tape_Delay_V1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HYLC_Tape_Delay_V1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HYLC_Tape_Delay_V1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // Clear output channels if more outputs than inputs
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    // Simple quiet audio for debugging:
    /*//Start==============================================================================
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* output = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            output[i] *= 0.1f; // Simple volume reduction
    }
    *///End==============================================================================
    
    // The real thing below:
    //Start==============================================================================
    // Validate and initialize circular buffer
    if (circularBuffer.empty() || bufferSize <= 0)
        return; // Exit if circular buffer is invalid

    // Process audio for each input channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* input = buffer.getReadPointer(channel);
        auto* output = buffer.getWritePointer(channel);
         
        for (int i = 0; i < numSamples; ++i)
        {
            // Write to the circular buffer
            circularBuffer[writePosition] = input[i];
         
            // Read from the circular buffer
            float delayedSample = circularBuffer[playbackPosition];
            output[i] = delayedSample; // Apply delay effect
         
            // Update positions with wraparound
            writePosition = (writePosition + 1) % bufferSize;
            playbackPosition = (playbackPosition + 1) % bufferSize;
        }
    }
    //End==============================================================================
    
    // Clear unused output channels
    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
        buffer.clear(channel, 0, numSamples);
}

//==============================================================================
bool HYLC_Tape_Delay_V1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HYLC_Tape_Delay_V1AudioProcessor::createEditor()
{
    return new HYLC_Tape_Delay_V1AudioProcessorEditor (*this);
}

//==============================================================================
void HYLC_Tape_Delay_V1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HYLC_Tape_Delay_V1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HYLC_Tape_Delay_V1AudioProcessor();
}

void HYLC_Tape_Delay_V1AudioProcessor::setLoopLength(float lengthInSeconds)
{
    bufferSize = static_cast<int>(getSampleRate() * lengthInSeconds);
    circularBuffer.resize(bufferSize, 0.0f); // Resize buffer and clear old data
    writePosition = 0;                       // Reset write position
    playbackPosition = 0;                    // Reset playback position
}
