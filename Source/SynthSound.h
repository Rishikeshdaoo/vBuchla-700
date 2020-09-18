/*
  ==============================================================================

    SynthSound.h
    Created: 26 Jan 2020 11:14:34am
    Author:  Rishikesh Daoo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public SynthesiserSound
{

public:
    bool appliesToNote (int midiNoteNumber) override;

    bool appliesToChannel (int midiNoteNumber) override;
};