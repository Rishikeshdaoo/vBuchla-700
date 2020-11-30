/*
  ==============================================================================

    BuchlaSynth.h
    Created: 29 Nov 2020 7:54:50pm
    Author:  Rishi D

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class BuchlaSynth : public juce::Synthesiser
{
public:
    static int  numOscillators;

    static void addADSRParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addOvertoneParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addGainParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);

    BuchlaSynth() = default;

    class BuchlaSound : public juce::SynthesiserSound
    {
    public:
        BuchlaSound (juce::AudioProcessorValueTreeState& state);
        bool appliesToNote (int) override { return true; }
        bool appliesToChannel (int) override { return true; }

        juce::ADSR::Parameters getVCA1();
        juce::ADSR::Parameters getVCA2();
        juce::ADSR::Parameters getVCA3();
        juce::ADSR::Parameters getVCA4();
        juce::ADSR::Parameters getVCA5();
    
    private:
        juce::AudioProcessorValueTreeState& state;
        juce::AudioParameterFloat* attack  = nullptr;
        juce::AudioParameterFloat* decay   = nullptr;
        juce::AudioParameterFloat* sustain = nullptr;
        juce::AudioParameterFloat* release = nullptr;
        
        juce::AudioParameterFloat* attack2  = nullptr;
        juce::AudioParameterFloat* decay2   = nullptr;
        juce::AudioParameterFloat* sustain2 = nullptr;
        juce::AudioParameterFloat* release2 = nullptr;

        juce::AudioParameterFloat* attack3  = nullptr;
        juce::AudioParameterFloat* decay3   = nullptr;
        juce::AudioParameterFloat* sustain3 = nullptr;
        juce::AudioParameterFloat* release3 = nullptr;

        juce::AudioParameterFloat* attack4  = nullptr;
        juce::AudioParameterFloat* decay4   = nullptr;
        juce::AudioParameterFloat* sustain4 = nullptr;
        juce::AudioParameterFloat* release4 = nullptr;

        juce::AudioParameterFloat* attack5  = nullptr;
        juce::AudioParameterFloat* decay5   = nullptr;
        juce::AudioParameterFloat* sustain5 = nullptr;
        juce::AudioParameterFloat* release5 = nullptr;

        juce::AudioParameterFloat* attack6  = nullptr;
        juce::AudioParameterFloat* decay6   = nullptr;
        juce::AudioParameterFloat* sustain6 = nullptr;
        juce::AudioParameterFloat* release6 = nullptr;
        
        juce::AudioParameterFloat* gain    = nullptr;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BuchlaSound)
    };

    class BuchlaVoice : public juce::SynthesiserVoice
    {
    public:
        BuchlaVoice (juce::AudioProcessorValueTreeState& state);

        bool canPlaySound (juce::SynthesiserSound *) override;

        void startNote (int midiNoteNumber,
                        float velocity,
                        juce::SynthesiserSound* sound,
                        int currentPitchWheelPosition) override;

        void stopNote (float velocity, bool allowTailOff) override;

        void pitchWheelMoved (int newPitchWheelValue) override;

        void controllerMoved (int controllerNumber, int newControllerValue) override;

        void renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                              int startSample,
                              int numSamples) override;

        void setCurrentPlaybackSampleRate (double newRate) override;

    private:

        class BaseOscillator
        {
        public:
            BaseOscillator() = default;

            juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>> osc;
            juce::AudioParameterFloat* gain   = nullptr;
            juce::AudioParameterFloat* detune = nullptr;
            double multiplier = 1.0;

        private:
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseOscillator)
        };

        double getDetuneFromPitchWheel (int wheelValue) const;
        double getFrequencyForNote (int noteNumber, double detune, double concertPitch = 440.0) const;

        void updateFrequency (BaseOscillator& oscillator, bool noteStart = false);

        std::vector<std::unique_ptr<BaseOscillator>> oscillators;

        double                      pitchWheelValue = 0.0;
        int                         maxPitchWheelSemitones = 12;
        const int                   internalBufferSize = 64;
        juce::AudioBuffer<float>    oscillatorBuffer;
        juce::AudioBuffer<float>    voiceBuffer;
        juce::ADSR                  vca1;
        juce::ADSR                  vca2;
        juce::ADSR                  vca3;
        juce::ADSR                  vca4;
        juce::ADSR                  vca5;
        juce::ADSR                  vca6;
        juce::AudioParameterFloat*  gainParameter = nullptr;
        float                       lastGain = 0.0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BuchlaVoice)
    };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BuchlaSynth)
};
