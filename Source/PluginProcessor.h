/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BuchlaSynth.h"

class PresetListBox;

//==============================================================================
/**
*/
class VBuchla700AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VBuchla700AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    void savePresetInternal();
    void loadPresetInternal(int index);

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

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

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState treeState;

        BuchlaSynth      synthesiser;
        juce::ValueTree  presetNode;

        // GUI MAGIC: define that as last member of your AudioProcessor
        foleys::MagicProcessorState magicState { *this, treeState };
        foleys::MagicLevelSource*   outputMeter  = nullptr;
        foleys::MagicPlotSource*    oscilloscope = nullptr;
        foleys::MagicPlotSource*    analyser     = nullptr;

        PresetListBox*              presetList   = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VBuchla700AudioProcessor)
};
