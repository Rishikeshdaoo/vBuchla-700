/*
  ==============================================================================

    SynthVoice.cpp
    Created: 1 Feb 2020 3:44:19pm
    Author:  Rishikesh Daoo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthVoice.h"

bool SynthVoice::canPlaySound (SynthesiserSound* sound){

    return dynamic_cast <SynthSound*>(sound) != nullptr;
}

void SynthVoice::setPitchBend (int pitchWheelPos){
    if (pitchWheelPos > 8192)
    {
        // shifting up
        pitchBend = float(pitchWheelPos - 8192) / (16383 - 8192);
    }
    else
    {
        // shifting down
        pitchBend = float(8192 - pitchWheelPos) / -8192;    // negative number
    }
}

float SynthVoice::pitchBendCents (){
    if (pitchBend >= 0.0f)
    {
        // shifting up
        return pitchBend * pitchBendUpSemitones * 100;
    }
    else
    {
        // shifting down
        return pitchBend * pitchBendDownSemitones * 100;
    }
}

double SynthVoice::noteHz (int midiNoteNumber, double centsOffset){

    double hertz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    hertz *= std::pow(2.0, centsOffset / 1200);
    return hertz;
}

void SynthVoice::getOscType (atomic<float>* selection){

    theWave = *selection;
}

void SynthVoice::getOsc2Type (atomic<float>* selection){

    theWave2 = *selection;
}

double SynthVoice::setOscType(){
    double sample1, sample2;

    switch (theWave)
    {
        case 0:
            sample1 = osc1.square(frequency);
            break;
        case 1:
            sample1 = osc1.saw(frequency);
            break;
        default:
            sample1 = osc1.sinewave(frequency);
            break;
    }

    switch (theWave2)
    {
        case 0:
            sample2 = osc2.saw(frequency / 2.0);
            break;
        case 1:
            sample2 = osc2.square(frequency / 2.0);
            break;
        default:
            sample2 = osc2.sinewave(frequency / 2.0);
            break;
    }

    return sample1 + osc2blend * sample2;
}

void SynthVoice::getEnvelopeParams (atomic<float> *attack, atomic<float> *decay, atomic<float> *sustain, atomic<float> *release){

    env1.setAttack(*attack);
    env1.setDecay(*decay);
    env1.setSustain(*sustain);
    env1.setRelease(*release);
}

double SynthVoice::setEnvelope (){

    return env1.adsr(setOscType(), env1.trigger);
}

void SynthVoice::getWillsParams (atomic<float>* mGain, atomic<float>* blend, atomic<float>* pbup, atomic<float>* pbdn){

    masterGain = *mGain;
    osc2blend = *blend;
    pitchBendUpSemitones = *pbup;
    pitchBendDownSemitones = *pbdn;
}

void SynthVoice::getFilterParams (atomic<float>* filterType, atomic<float>* filterCutoff, atomic<float>* filterRes){

    filterChoice = *filterType;
    cutoff = *filterCutoff;
    resonance = *filterRes;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)    {

    noteNumber = midiNoteNumber;
    env1.trigger = 1;
    setPitchBend(currentPitchWheelPosition);
    frequency = noteHz(noteNumber, pitchBendCents());
    level = velocity;
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)    {

    env1.trigger = 0;
    allowTailOff = true;

    if (velocity == 0)
        clearCurrentNote();
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue){

    setPitchBend(newPitchWheelValue);
    frequency = noteHz(noteNumber, pitchBendCents());
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue){

}

void SynthVoice::renderNextBlock (AudioBuffer <float> &outputBuffer, int startSample, int numSamples){

    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample, setEnvelope() * masterGain);
        }
        ++startSample;
    }
}

