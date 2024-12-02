/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
//Nate to add
#include <JuceHeader.h>

//==============================================================================
/**
*/
class HYLC_Tape_Delay_V1AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    HYLC_Tape_Delay_V1AudioProcessor();
    ~HYLC_Tape_Delay_V1AudioProcessor() override;

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
    
    void setLoopLength(float lengthInSeconds);
    
    float getLoopLength() const { return static_cast<float>(bufferSize) / getSampleRate(); }

private:
    std::vector<float> circularBuffer;  // Circular buffer for storing audio
        int bufferSize = 0;                 // Size of the circular buffer
        int writePosition = 0;              // Current write position in the buffer
        int playbackPosition = 0;           // Current playback position
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HYLC_Tape_Delay_V1AudioProcessor)
};
