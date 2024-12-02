/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HYLC_Tape_Delay_V1AudioProcessorEditor::HYLC_Tape_Delay_V1AudioProcessorEditor (HYLC_Tape_Delay_V1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set the range and step size for the slider
    sliderOne.setRange(1.0, 10.0, 0.1); // Loop length: 1 to 10 seconds
    sliderOne.setValue(audioProcessor.getLoopLength());            // Default value: 5 seconds

    // Set the slider style
    sliderOne.setSliderStyle(juce::Slider::LinearHorizontal);
    sliderOne.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    // Attach a callback for when the slider value changes
    sliderOne.onValueChange = [this]()
    {
        audioProcessor.setLoopLength(sliderOne.getValue()); // Notify the processor
    };

    // Add the slider to the editor's visible components
    addAndMakeVisible(sliderOne);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

HYLC_Tape_Delay_V1AudioProcessorEditor::~HYLC_Tape_Delay_V1AudioProcessorEditor()
{
}

//==============================================================================
void HYLC_Tape_Delay_V1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(232, 227, 228));

    g.setColour (juce::Colour(76, 112, 48));
    //g.setFont (15.0f);
    g.setFont(juce::Font("Courier", 15.0f, juce::Font::plain));
    //g.drawFittedText ("HYLLLIC TAPE DELAY V.1", getLocalBounds(), juce::Justification::centred, 1);
    g.drawText("HYLC TAPE DELAY v.1",
                   getLocalBounds().reduced(10, 10),
                   juce::Justification::topRight,
                   true);
}

void HYLC_Tape_Delay_V1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // Set the slider's bounds to occupy the bottom half of the window
    auto area = getLocalBounds();                // Get the full window bounds
    sliderOne.setBounds(area.removeFromBottom(50).reduced(10)); // Reduce margins for padding
}
