/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TouchyPitcherAudioProcessor::TouchyPitcherAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        m_ValueTreeState(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

TouchyPitcherAudioProcessor::~TouchyPitcherAudioProcessor()
{
}

//==============================================================================
const String TouchyPitcherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TouchyPitcherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TouchyPitcherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TouchyPitcherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TouchyPitcherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TouchyPitcherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TouchyPitcherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TouchyPitcherAudioProcessor::setCurrentProgram (int index)
{
}

const String TouchyPitcherAudioProcessor::getProgramName (int index)
{
    return {};
}

void TouchyPitcherAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TouchyPitcherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    m_SoundTouch.setSampleRate(sampleRate);
    m_SoundTouch.setPitchSemiTones(0.0f);
    m_SoundTouch.setChannels(1);
    m_SoundTouch.flush();
    
    m_ProcessedBuffer.resize(samplesPerBlock);
}

void TouchyPitcherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TouchyPitcherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void TouchyPitcherAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    // Params we'll be using throughout the block
    int numSamples = buffer.getNumSamples();
    float pitch = m_ValueTreeState.getRawParameterValue("PITCH")->load();
    
    // Set SoundTouch pitch and input samples
    // Only grabbing 1 channel
    // TODO: Stereo interleaving
    m_SoundTouch.setPitchSemiTones(pitch);
    m_SoundTouch.putSamples(buffer.getReadPointer(0), numSamples);
    
    // Output
    if (m_SoundTouch.numSamples() >= numSamples)
    {
        // Receive processed output if SoundTouch has enough samples
        int numOutputSamples = 0;
        numOutputSamples = m_SoundTouch.receiveSamples(m_ProcessedBuffer.data(), numSamples);

        // Copy to all channels
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            // auto* channelData = buffer.getWritePointer (channel);
            buffer.copyFrom(channel, 0, m_ProcessedBuffer.data(), numSamples);
        }
    }
    else
    {
        // Otherwise, just output silence for this block
        buffer.clear();
    }
}

//==============================================================================
bool TouchyPitcherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TouchyPitcherAudioProcessor::createEditor()
{
    return new TouchyPitcherAudioProcessorEditor (*this);
}

//==============================================================================
void TouchyPitcherAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TouchyPitcherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout TouchyPitcherAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    
    parameters.push_back(std::make_unique<AudioParameterFloat>("PITCH", "Pitch (semitones)", -12.0f, 12.0f, 0.0f));
    
    return {parameters.begin(), parameters.end() };
}

void TouchyPitcherAudioProcessor::valueTreePropertyChanged(ValueTree &treeWhosePropertyhasChanged, const Identifier &property)
{
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TouchyPitcherAudioProcessor();
}
