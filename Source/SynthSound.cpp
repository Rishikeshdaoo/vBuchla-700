/*
  ==============================================================================

    SynthSound.cpp
    Created: 26 Jan 2020 11:14:34am
    Author:  Rishikesh Daoo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthSound.h"

bool SynthSound::appliesToNote (int midiNoteNumber){

    return true;
}

bool SynthSound::appliesToChannel (int midiNoteNumber){

    return true;
}