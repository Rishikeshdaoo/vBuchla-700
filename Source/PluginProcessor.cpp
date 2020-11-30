/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PresetListBox.h"

//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    BuchlaSynth::addADSRParameters (layout);
    BuchlaSynth::addOvertoneParameters (layout);
    BuchlaSynth::addGainParameters (layout);
    return layout;
}

//==============================================================================

VBuchla700AudioProcessor::VBuchla700AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : juce::AudioProcessor (juce::AudioProcessor::BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
    treeState (*this, nullptr, ProjectInfo::projectName, createParameterLayout())
{
    // MAGIC GUI: add a meter at the output
    outputMeter  = magicState.createAndAddObject<foleys::MagicLevelSource>("output");
    oscilloscope = magicState.createAndAddObject<foleys::MagicOscilloscope>("waveform");
    analyser     = magicState.createAndAddObject<foleys::MagicAnalyser>("analyser");
    magicState.addBackgroundProcessing (analyser);

    presetNode = magicState.getValueTreeState().state.getOrCreateChildWithName ("presets", nullptr);

    presetList = magicState.createAndAddObject<PresetListBox>("presets", presetNode);
    presetList->onSelectionChanged = [&](int number)
    {
        loadPresetInternal (number);
    };
    magicState.addTrigger ("save-preset", [this]
    {
        savePresetInternal();
    });

    magicState.setApplicationSettingsFile (juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                                           .getChildFile (ProjectInfo::companyName)
                                           .getChildFile (ProjectInfo::projectName + juce::String (".settings")));

    magicState.setPlayheadUpdateFrequency (30);

    BuchlaSynth::BuchlaSound::Ptr sound (new BuchlaSynth::BuchlaSound (treeState));
    synthesiser.addSound (sound);

    for (int i=0; i < 16; ++i)
        synthesiser.addVoice (new BuchlaSynth::BuchlaVoice (treeState));
}

//==============================================================================
void VBuchla700AudioProcessor::prepareToPlay (double sampleRate, int blockSize)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synthesiser.setCurrentPlaybackSampleRate (sampleRate);

    // MAGIC GUI: setup the output meter
    outputMeter->setupSource (getTotalNumOutputChannels(), sampleRate, 500, 200);
    oscilloscope->prepareToPlay (sampleRate, blockSize);
    analyser->prepareToPlay (sampleRate, blockSize);
}

void VBuchla700AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VBuchla700AudioProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void VBuchla700AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // MAGIC GUI: send midi messages to the keyboard state
    magicState.processMidiBuffer (midiMessages, buffer.getNumSamples(), true);

    // MAGIC GUI: send playhead information to the GUI
    magicState.updatePlayheadInformation (getPlayHead());

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synthesiser.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    for (int i = 1; i < buffer.getNumChannels(); ++i)
        buffer.copyFrom (i, 0, buffer.getReadPointer (0), buffer.getNumSamples());

    // MAGIC GUI: send the finished buffer to the level meter
    outputMeter->pushSamples (buffer);
    oscilloscope->pushSamples (buffer);
    analyser->pushSamples (buffer);
}

//==============================================================================
bool VBuchla700AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VBuchla700AudioProcessor::createEditor()
{
    // MAGIC GUI: create the generated editor
    return new foleys::MagicPluginEditor (magicState, BinaryData::buchla700_xml, BinaryData::buchla700_xmlSize);
}

//==============================================================================
void VBuchla700AudioProcessor::savePresetInternal()
{
    juce::ValueTree preset { "Preset" };
    preset.setProperty ("name", "Preset " + juce::String (presetNode.getNumChildren() + 1), nullptr);
    for (const auto& p : magicState.getValueTreeState().state)
        if (p.getType().toString() == "PARAM")
            preset.appendChild (p.createCopy(), nullptr);

    presetNode.appendChild (preset, nullptr);

    presetList->sendChangeMessage();
}

void VBuchla700AudioProcessor::loadPresetInternal(int index)
{
    auto preset = presetNode.getChild (index);
    for (const auto& p : preset)
    {
        if (p.hasType ("PARAM"))
        {
            auto id = p.getProperty ("id", "unknownID").toString();
            if (auto* parameter = dynamic_cast<juce::RangedAudioParameter*>(magicState.getValueTreeState().getParameter (id)))
                parameter->setValueNotifyingHost (parameter->convertTo0to1 (p.getProperty ("value")));
        }
    }
}

//==============================================================================
void VBuchla700AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // MAGIC GUI: let the magicState conveniently handle save and restore the state.
    //            You don't need to use that, but it also takes care of restoring the last editor size
    magicState.getStateInformation (destData);
}

void VBuchla700AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // MAGIC GUI: let the magicState conveniently handle save and restore the state.
    //            You don't need to use that, but it also takes care of restoring the last editor size
    magicState.setStateInformation (data, sizeInBytes, getActiveEditor());

    presetNode = magicState.getValueTreeState().state.getOrCreateChildWithName ("presets", nullptr);
    presetList->setPresetsNode (presetNode);
}

//==============================================================================
const juce::String VBuchla700AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VBuchla700AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VBuchla700AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VBuchla700AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VBuchla700AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VBuchla700AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VBuchla700AudioProcessor::getCurrentProgram()
{
    return 0;
}

void VBuchla700AudioProcessor::setCurrentProgram (int index)
{
    loadPresetInternal (index);
}

const juce::String VBuchla700AudioProcessor::getProgramName (int)
{
    return {};
}

void VBuchla700AudioProcessor::changeProgramName (int, const juce::String&)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VBuchla700AudioProcessor();
}
