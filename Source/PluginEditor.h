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
class HYLC_Tape_Delay_V1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HYLC_Tape_Delay_V1AudioProcessorEditor (HYLC_Tape_Delay_V1AudioProcessor&);
    ~HYLC_Tape_Delay_V1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HYLC_Tape_Delay_V1AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HYLC_Tape_Delay_V1AudioProcessorEditor)
};
