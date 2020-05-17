/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TouchyPitcherAudioProcessorEditor::TouchyPitcherAudioProcessorEditor (TouchyPitcherAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    m_PitchSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    m_PitchSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 45, 25);
    addAndMakeVisible(m_PitchSlider);
    
    m_PitchLabel.setText("Pitch (semitones)", NotificationType::dontSendNotification);
    m_PitchLabel.setJustificationType(Justification::centredTop);
    m_PitchLabel.attachToComponent(&m_PitchSlider, false);
    
    m_PitchAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "PITCH", m_PitchSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

TouchyPitcherAudioProcessorEditor::~TouchyPitcherAudioProcessorEditor()
{
}

//==============================================================================
void TouchyPitcherAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void TouchyPitcherAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    int w = 150;
    int h = 100;
    int x = (getWidth() * 0.5f) - (w * 0.5f);
    int y = (getHeight() * 0.5f) - (h * 0.5f);
    
    m_PitchSlider.setBounds(x, y, w, h);
}
