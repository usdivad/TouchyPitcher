/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TouchyPitcherAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    TouchyPitcherAudioProcessorEditor (TouchyPitcherAudioProcessor&);
    ~TouchyPitcherAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TouchyPitcherAudioProcessor& processor;
    
    //==============================================================================
    Slider m_PitchSlider;
    Label m_PitchLabel;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_PitchAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TouchyPitcherAudioProcessorEditor)
};
