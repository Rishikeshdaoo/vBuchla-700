/*
  ==============================================================================

    SynthVoice.h
    Created: 1 Feb 2020 3:44:19pm
    Author:  Rishikesh Daoo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "maximilian.h"

class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound (SynthesiserSound* sound) override;

    void setPitchBend(int pitchWheelPos);

    float pitchBendCents();

    static double noteHz(int midiNoteNumber, double centsOffset);

    //=======================================================

    void getOscType(atomic<float>* selection);

    void getOsc2Type(atomic<float>* selection);

    //=======================================================

    double setOscType ();

    //=======================================================

    void getEnvelopeParams(atomic<float> *attack, atomic<float> *decay, atomic<float> *sustain, atomic<float> *release);

    //=======================================================

    double setEnvelope();

    //=======================================================

    void getWillsParams(atomic<float>* mGain, atomic<float>* blend, atomic<float>* pbup, atomic<float>* pbdn);

    void getFilterParams (atomic<float>* filterType, atomic<float>* filterCutoff, atomic<float>* filterRes);

    //=======================================================

    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;

    //=======================================================

    void stopNote (float velocity, bool allowTailOff) override;

    //=======================================================

    void pitchWheelMoved (int newPitchWheelValue) override;

    //=======================================================

    void controllerMoved (int controllerNumber, int newControllerValue) override;

    //=======================================================

    void renderNextBlock (AudioBuffer <float> &outputBuffer, int startSample, int numSamples) override;

    //=======================================================
private:
    double level;
    double frequency;
    int theWave, theWave2;

    float masterGain;
    float osc2blend;

    int noteNumber;
    float pitchBend = 0.0f;
    float pitchBendUpSemitones = 2.0f;
    float pitchBendDownSemitones = 2.0f;

    int filterChoice;
    float cutoff;
    float resonance;

    maxiOsc osc1, osc2;
    maxiEnv env1;
};
