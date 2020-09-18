/*
  ==============================================================================

    Frontgui.h
    Created: 01 Mar 2020 6:45:40pm
    Author:  Rishikesh Daoo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
 */

class Frontgui    : public Component
{
public:
    Frontgui(JuceSynthFrameworkAudioProcessor&);
    ~Frontgui();

    void paint (Graphics&) override;
    void resized() override;


private:
    Slider mastergainSlider;
    Slider pbupSlider;
    Slider pbdownSlider;


    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> mastergainVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> pbupVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> pbdownVal;



    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceSynthFrameworkAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Frontgui)
};