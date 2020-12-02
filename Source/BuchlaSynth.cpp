/*
  ==============================================================================

    BuchlaSynth.cpp
    Created: 10 Nov 2020 7:54:50pm
    Author:  Rishikesh Daoo

  ==============================================================================
*/

#include "BuchlaSynth.h"

namespace IDs
{
    static juce::String paramAttack  { "attack" };
    static juce::String paramDecay   { "decay" };
    static juce::String paramSustain { "sustain" };
    static juce::String paramRelease { "release" };

    static juce::String paramAttack2  { "attack2" };
    static juce::String paramDecay2   { "decay2" };
    static juce::String paramSustain2 { "sustain2" };
    static juce::String paramRelease2 { "release2" };

    static juce::String paramAttack3  { "attack3" };
    static juce::String paramDecay3   { "decay3" };
    static juce::String paramSustain3 { "sustain3" };
    static juce::String paramRelease3 { "release3" };

    static juce::String paramAttack4  { "attack4" };
    static juce::String paramDecay4   { "decay4" };
    static juce::String paramSustain4 { "sustain4" };
    static juce::String paramRelease4 { "release4" };

    static juce::String paramAttack5  { "attack5" };
    static juce::String paramDecay5   { "decay5" };
    static juce::String paramSustain5 { "sustain5" };
    static juce::String paramRelease5 { "release5" };

    static juce::String paramAttack6  { "attack6" };
    static juce::String paramDecay6   { "decay6" };
    static juce::String paramSustain6 { "sustain6" };
    static juce::String paramRelease6 { "release6" };

    static juce::String paramGain    { "gain" };
}

//==============================================================================

int BuchlaSynth::numOscillators = 4;

void BuchlaSynth::addADSRParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto attack  = std::make_unique<juce::AudioParameterFloat>(IDs::paramAttack,  "Attack",  juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto decay   = std::make_unique<juce::AudioParameterFloat>(IDs::paramDecay,   "Decay",   juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto sustain = std::make_unique<juce::AudioParameterFloat>(IDs::paramSustain, "Sustain", juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f);
    auto release = std::make_unique<juce::AudioParameterFloat>(IDs::paramRelease, "Release", juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);

    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("adsr", "ADSR1", "|",
                                                                      std::move (attack),
                                                                      std::move (decay),
                                                                      std::move (sustain),
                                                                      std::move (release));
    layout.add (std::move (group));
    
    auto attack2  = std::make_unique<juce::AudioParameterFloat>(IDs::paramAttack2,  "Attack",  juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto decay2   = std::make_unique<juce::AudioParameterFloat>(IDs::paramDecay2,   "Decay",   juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto sustain2 = std::make_unique<juce::AudioParameterFloat>(IDs::paramSustain2, "Sustain", juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f);
    auto release2 = std::make_unique<juce::AudioParameterFloat>(IDs::paramRelease2, "Release", juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);

    auto group2 = std::make_unique<juce::AudioProcessorParameterGroup>("adsr2", "ADSR2", "|",
                                                                      std::move (attack2),
                                                                      std::move (decay2),
                                                                      std::move (sustain2),
                                                                      std::move (release2));
    layout.add (std::move (group2));
    
    auto attack3  = std::make_unique<juce::AudioParameterFloat>(IDs::paramAttack3,  "Attack",  juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto decay3   = std::make_unique<juce::AudioParameterFloat>(IDs::paramDecay3,   "Decay",   juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto sustain3 = std::make_unique<juce::AudioParameterFloat>(IDs::paramSustain3, "Sustain", juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f);
    auto release3 = std::make_unique<juce::AudioParameterFloat>(IDs::paramRelease3, "Release", juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);

    auto group3 = std::make_unique<juce::AudioProcessorParameterGroup>("adsr3", "ADSR3", "|",
                                                                      std::move (attack3),
                                                                      std::move (decay3),
                                                                      std::move (sustain3),
                                                                      std::move (release3));
    layout.add (std::move (group3));
    
    auto attack4  = std::make_unique<juce::AudioParameterFloat>(IDs::paramAttack4,  "Attack",  juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto decay4   = std::make_unique<juce::AudioParameterFloat>(IDs::paramDecay4,   "Decay",   juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto sustain4 = std::make_unique<juce::AudioParameterFloat>(IDs::paramSustain4, "Sustain", juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f);
    auto release4 = std::make_unique<juce::AudioParameterFloat>(IDs::paramRelease4, "Release", juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);

    auto group4 = std::make_unique<juce::AudioProcessorParameterGroup>("adsr4", "ADSR4", "|",
                                                                      std::move (attack4),
                                                                      std::move (decay4),
                                                                      std::move (sustain4),
                                                                      std::move (release4));
    layout.add (std::move (group4));
    
    auto attack5  = std::make_unique<juce::AudioParameterFloat>(IDs::paramAttack5,  "Attack",  juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto decay5   = std::make_unique<juce::AudioParameterFloat>(IDs::paramDecay5,   "Decay",   juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto sustain5 = std::make_unique<juce::AudioParameterFloat>(IDs::paramSustain5, "Sustain", juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f);
    auto release5 = std::make_unique<juce::AudioParameterFloat>(IDs::paramRelease5, "Release", juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);

    auto group5 = std::make_unique<juce::AudioProcessorParameterGroup>("adsr5", "ADSR5", "|",
                                                                      std::move (attack5),
                                                                      std::move (decay5),
                                                                      std::move (sustain5),
                                                                      std::move (release5));
    layout.add (std::move (group5));
    
    auto attack6  = std::make_unique<juce::AudioParameterFloat>(IDs::paramAttack6,  "Attack",  juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto decay6   = std::make_unique<juce::AudioParameterFloat>(IDs::paramDecay6,   "Decay",   juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto sustain6 = std::make_unique<juce::AudioParameterFloat>(IDs::paramSustain6, "Sustain", juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f);
    auto release6 = std::make_unique<juce::AudioParameterFloat>(IDs::paramRelease6, "Release", juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);

    auto group6 = std::make_unique<juce::AudioProcessorParameterGroup>("adsr6", "ADSR6", "|",
                                                                      std::move (attack6),
                                                                      std::move (decay6),
                                                                      std::move (sustain6),
                                                                      std::move (release6));
    layout.add (std::move (group6));
}

void BuchlaSynth::addOvertoneParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("oscillators", "Oscillators", "|");
    for (int i = 0; i < BuchlaSynth::numOscillators; ++i)
    {
        group->addChild (std::make_unique<juce::AudioParameterFloat>("osc" + juce::String (i), "Oscillator " + juce::String (i), juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
        group->addChild (std::make_unique<juce::AudioParameterFloat>("detune" + juce::String (i), "Detune " + juce::String (i), juce::NormalisableRange<float>(-0.5f, 0.5f, 0.01f), 0.0f));
    }

    layout.add (std::move (group));
}

void BuchlaSynth::addGainParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto gain  = std::make_unique<juce::AudioParameterFloat>(IDs::paramGain,  "Gain",  juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.70f);

    layout.add (std::make_unique<juce::AudioProcessorParameterGroup>("output", "Output", "|", std::move (gain)));
}

//==============================================================================

BuchlaSynth::BuchlaSound::BuchlaSound (juce::AudioProcessorValueTreeState& stateToUse)
  : state (stateToUse)
{
    attack = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramAttack));
    jassert (attack);
    decay = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramDecay));
    jassert (decay);
    sustain = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramSustain));
    jassert (sustain);
    release = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramRelease));
    jassert (release);
    
    attack2 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramAttack2));
    jassert (attack2);
    decay2 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramDecay2));
    jassert (decay2);
    sustain2 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramSustain2));
    jassert (sustain2);
    release2 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramRelease2));
    jassert (release2);
    
    attack3 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramAttack3));
    jassert (attack3);
    decay3 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramDecay3));
    jassert (decay3);
    sustain3 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramSustain3));
    jassert (sustain3);
    release3 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramRelease3));
    jassert (release3);
    
    attack4 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramAttack4));
    jassert (attack4);
    decay4 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramDecay4));
    jassert (decay4);
    sustain4 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramSustain4));
    jassert (sustain4);
    release4 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramRelease4));
    jassert (release4);
    
    attack5 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramAttack5));
    jassert (attack5);
    decay5 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramDecay5));
    jassert (decay5);
    sustain5 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramSustain5));
    jassert (sustain5);
    release5 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramRelease5));
    jassert (release5);
    
    attack6 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramAttack6));
    jassert (attack6);
    decay6 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramDecay6));
    jassert (decay6);
    sustain6 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramSustain6));
    jassert (sustain6);
    release6 = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramRelease6));
    jassert (release6);
    
    gain = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramGain));
    jassert (gain);
}

juce::ADSR::Parameters BuchlaSynth::BuchlaSound::getVCA1()
{
    juce::ADSR::Parameters parameters;
    parameters.attack  = attack->get();
    parameters.decay   = decay->get();
    parameters.sustain = sustain->get();
    parameters.release = release->get();

    return parameters;
}

juce::ADSR::Parameters BuchlaSynth::BuchlaSound::getVCA2()
{
    juce::ADSR::Parameters parameters;
    parameters.attack  = attack2->get();
    parameters.decay   = decay2->get();
    parameters.sustain = sustain2->get();
    parameters.release = release2->get();

    return parameters;
}

juce::ADSR::Parameters BuchlaSynth::BuchlaSound::getVCA3()
{
    juce::ADSR::Parameters parameters;
    parameters.attack  = attack3->get();
    parameters.decay   = decay3->get();
    parameters.sustain = sustain3->get();
    parameters.release = release3->get();

    return parameters;
}

juce::ADSR::Parameters BuchlaSynth::BuchlaSound::getVCA4()
{
    juce::ADSR::Parameters parameters;
    parameters.attack  = attack4->get();
    parameters.decay   = decay4->get();
    parameters.sustain = sustain4->get();
    parameters.release = release4->get();

    return parameters;
}

juce::ADSR::Parameters BuchlaSynth::BuchlaSound::getVCA5()
{
    juce::ADSR::Parameters parameters;
    parameters.attack  = attack5->get();
    parameters.decay   = decay5->get();
    parameters.sustain = sustain5->get();
    parameters.release = release5->get();

    return parameters;
}

juce::ADSR::Parameters BuchlaSynth::BuchlaSound::getVCA6()
{
    juce::ADSR::Parameters parameters;
    parameters.attack  = attack6->get();
    parameters.decay   = decay6->get();
    parameters.sustain = sustain6->get();
    parameters.release = release6->get();

    return parameters;
}

//==============================================================================

BuchlaSynth::BuchlaVoice::BuchlaVoice (juce::AudioProcessorValueTreeState& state)
{
    for (int i=0; i < BuchlaSynth::numOscillators; ++i)
    {
        oscillators.push_back (std::make_unique<BaseOscillator>());
        auto& osc = oscillators.back();
        osc->gain = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter ("osc" + juce::String (i)));
        osc->detune = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter ("detune" + juce::String (i)));
        osc->osc.get<0>().initialise ([](auto arg){return std::sin (arg);}, 512);
        osc->multiplier = i + 1;
    }

    gainParameter = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter (IDs::paramGain));
    jassert (gainParameter);

    oscillatorBuffer1.setSize (1, internalBufferSize);
    oscillatorBuffer2.setSize (1, internalBufferSize);
    oscillatorBuffer3.setSize (1, internalBufferSize);
    oscillatorBuffer4.setSize (1, internalBufferSize);
    
    vcaBuffer1.setSize (1, internalBufferSize);
    vcaBuffer2.setSize (1, internalBufferSize);
    vcaBuffer3.setSize (1, internalBufferSize);
    vcaBuffer4.setSize (1, internalBufferSize);
    vcaBuffer5.setSize (1, internalBufferSize);
    vcaBuffer6.setSize (1, internalBufferSize);
    
    // use a default samplerate and vector size here, reset it later
    m_C74PluginState = (CommonState *)C74_GENPLUGIN::create(44100, 64);
    C74_GENPLUGIN::reset(m_C74PluginState);

    m_InputBuffers = new t_sample *[C74_GENPLUGIN::num_inputs()];
    m_OutputBuffers = new t_sample *[C74_GENPLUGIN::num_outputs()];
    
//    for (int i = 0; i < C74_GENPLUGIN::num_inputs(); i++) {
//        m_InputBuffers[i] = NULL;
//    }
//    for (int i = 0; i < C74_GENPLUGIN::num_outputs(); i++) {
//        m_OutputBuffers[i] = NULL;
//    }
}

bool BuchlaSynth::BuchlaVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<BuchlaSound*>(sound) != nullptr;
}

void BuchlaSynth::BuchlaVoice::startNote ([[maybe_unused]]int midiNoteNumber,
                                          [[maybe_unused]]float velocity,
                                          juce::SynthesiserSound* sound,
                                          int currentPitchWheelPosition)
{
    if (auto* buchlaSound = dynamic_cast<BuchlaSound*>(sound))
        vca1.setParameters (buchlaSound->getVCA1());
    
    if (auto* buchlaSound = dynamic_cast<BuchlaSound*>(sound))
        vca2.setParameters (buchlaSound->getVCA2());
    
    if (auto* buchlaSound = dynamic_cast<BuchlaSound*>(sound))
        vca3.setParameters (buchlaSound->getVCA3());
    
    if (auto* buchlaSound = dynamic_cast<BuchlaSound*>(sound))
        vca4.setParameters (buchlaSound->getVCA4());
    
    if (auto* buchlaSound = dynamic_cast<BuchlaSound*>(sound))
        vca5.setParameters (buchlaSound->getVCA5());
    
    if (auto* buchlaSound = dynamic_cast<BuchlaSound*>(sound))
        vca6.setParameters (buchlaSound->getVCA6());
    
    pitchWheelValue = getDetuneFromPitchWheel (currentPitchWheelPosition);

//    vca1.noteOn();
//    vca2.noteOn();
//    vca3.noteOn();
//    vca4.noteOn();
//    vca5.noteOn();
//    vca6.noteOn();
    
    
    
    for (auto& osc : oscillators)
        updateFrequency (*osc, true);
}

void BuchlaSynth::BuchlaVoice::stopNote ([[maybe_unused]]float velocity,
                                         bool allowTailOff)
{
    if (allowTailOff)
    {
        vca1.noteOff();
        vca2.noteOff();
        vca3.noteOff();
        vca4.noteOff();
        vca5.noteOff();
        vca6.noteOff();
    }
    else
    {
        vca1.reset();
        vca2.reset();
        vca3.reset();
        vca4.reset();
        vca5.reset();
        vca6.reset();
        clearCurrentNote();
    }

    clearCurrentNote();
}

void BuchlaSynth::BuchlaVoice::pitchWheelMoved (int newPitchWheelValue)
{
    pitchWheelValue = getDetuneFromPitchWheel (newPitchWheelValue);
}

void BuchlaSynth::BuchlaVoice::controllerMoved ([[maybe_unused]]int controllerNumber, [[maybe_unused]]int newControllerValue)
{
}

void BuchlaSynth::BuchlaVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                                                int startSample,
                                                int numSamples)
{
    
    if (! vca1.isActive())
        return;

    while (numSamples > 0)
    {
        auto left = std::min (numSamples, oscillatorBuffer1.getNumSamples());
        
        auto block1 = juce::dsp::AudioBlock<float> (oscillatorBuffer1).getSingleChannelBlock (0).getSubBlock (0, size_t (left));
        auto block2 = juce::dsp::AudioBlock<float> (oscillatorBuffer1).getSingleChannelBlock (0).getSubBlock (0, size_t (left));
        auto block3 = juce::dsp::AudioBlock<float> (oscillatorBuffer1).getSingleChannelBlock (0).getSubBlock (0, size_t (left));
        auto block4 = juce::dsp::AudioBlock<float> (oscillatorBuffer1).getSingleChannelBlock (0).getSubBlock (0, size_t (left));
        
        juce::dsp::ProcessContextReplacing<float> context1 (block1);
        juce::dsp::ProcessContextReplacing<float> context2 (block2);
        juce::dsp::ProcessContextReplacing<float> context3 (block3);
        juce::dsp::ProcessContextReplacing<float> context4 (block4);
        
        auto& osc1 = oscillators[0];
        auto oscGain = osc1->gain->get();
        if (oscGain < 0.01)
            continue;
        updateFrequency (*osc1);
        osc1->osc.get<1>().setGainLinear (oscGain);
        oscillatorBuffer1.clear();
        osc1->osc.process (context1);
        
        auto& osc2 = oscillators[0];
        auto oscGain2 = osc2->gain->get();
        if (oscGain2 < 0.01)
            continue;
        updateFrequency (*osc2);
        osc2->osc.get<1>().setGainLinear (oscGain);
        oscillatorBuffer1.clear();
        osc2->osc.process (context1);
        
        auto& osc3 = oscillators[0];
        auto oscGain3 = osc3->gain->get();
        if (oscGain3 < 0.01)
            continue;
        updateFrequency (*osc1);
        osc3->osc.get<1>().setGainLinear (oscGain);
        oscillatorBuffer1.clear();
        osc3->osc.process (context1);
        
        auto& osc4 = oscillators[0];
        auto oscGain4 = osc4->gain->get();
        if (oscGain4 < 0.01)
            continue;
        updateFrequency (*osc1);
        osc4->osc.get<1>().setGainLinear (oscGain);
        oscillatorBuffer1.clear();
        osc4->osc.process (context1);
        
        // create fake vcas
        vca1.applyEnvelopeToBuffer (vcaBuffer1, 0, left);
        vca2.applyEnvelopeToBuffer (vcaBuffer2, 0, left);
        vca3.applyEnvelopeToBuffer (vcaBuffer3, 0, left);
        vca4.applyEnvelopeToBuffer (vcaBuffer4, 0, left);
        vca5.applyEnvelopeToBuffer (vcaBuffer5, 0, left);
        vca6.applyEnvelopeToBuffer (vcaBuffer6, 0, left);
        
        std::cout << vcaBuffer1.getSample(0, 100);
        
        for(int i=0; i < C74_GENPLUGIN::num_inputs(); ++i){
            m_InputBuffers[i] = new t_sample[left]();
            }
        
        for(int i=0; i < C74_GENPLUGIN::num_outputs(); ++i){
            m_OutputBuffers[i] = new t_sample[left]();
            }
        
        // GEN processing?: 10xgetNumSamples - 2xgetNumSamples
        // fill input buffers by using 4 oscillatorBuffers and 6 adsrs
        for (int j = 0; j < left; j++) {
            m_InputBuffers[0][j] = oscillatorBuffer1.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[1][j] = oscillatorBuffer2.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[2][j] = oscillatorBuffer3.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[3][j] = oscillatorBuffer4.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[4][j] = vcaBuffer1.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[5][j] = vcaBuffer2.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[6][j] = vcaBuffer3.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[7][j] = vcaBuffer4.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[8][j] = vcaBuffer5.getReadPointer(0)[j];
        }
        for (int j = 0; j < left; j++) {
            m_InputBuffers[9][j] = vcaBuffer6.getReadPointer(0)[j];
        }
        
        // process audio : 10x512 - 2x512
        C74_GENPLUGIN::perform(m_C74PluginState,
                                      m_InputBuffers,
                                      C74_GENPLUGIN::num_inputs(),
                                      m_OutputBuffers,
                                      C74_GENPLUGIN::num_outputs(),
                                      oscillatorBuffer1.getNumSamples());

        // fill output buffers into internal JUCE buffer
        for (int i = 0; i < oscillatorBuffer1.getNumChannels(); i++) {
            if (i < C74_GENPLUGIN::num_outputs()) {
                for (int j = 0; j < left; j++) {
                    outputBuffer.getWritePointer(i)[j] = m_OutputBuffers[i][j];
                }
            } else {
                outputBuffer.clear (i, 0, oscillatorBuffer1.getNumSamples());
            }
        }
        const auto gain = gainParameter->get();
        // final output gets rid of pops
        outputBuffer.addFromWithRamp (0, startSample, outputBuffer.getReadPointer (0), left, lastGain, gain);
        lastGain = gain;

        startSample += left;
        numSamples  -= left;

        if (! vca1.isActive())
            clearCurrentNote();
    }
}

void BuchlaSynth::BuchlaVoice::setCurrentPlaybackSampleRate (double newRate)
{
    juce::SynthesiserVoice::setCurrentPlaybackSampleRate (newRate);

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = newRate;
    spec.maximumBlockSize = juce::uint32 (internalBufferSize);
    spec.numChannels = 1;
    for (auto& osc : oscillators)
        osc->osc.prepare (spec);
}

double BuchlaSynth::BuchlaVoice::getFrequencyForNote (int noteNumber, double detune, double concertPitch) const
{
    return concertPitch * std::pow (2.0, (noteNumber + detune - 69.0) / 12.0);
}

double BuchlaSynth::BuchlaVoice::getDetuneFromPitchWheel (int wheelValue) const
{
    return (wheelValue / 8192.0) - 1.0;
}

void BuchlaSynth::BuchlaVoice::updateFrequency (BaseOscillator& oscillator, bool noteStart)
{
    const auto freq = getFrequencyForNote (getCurrentlyPlayingNote(),
                                           pitchWheelValue * maxPitchWheelSemitones
                                           + oscillator.detune->get());
    oscillator.osc.get<0>().setFrequency (float (freq * oscillator.multiplier), noteStart);
}


void BuchlaSynth::BuchlaVoice::assureBufferSize(long bufferSize)
{
    if (bufferSize > m_CurrentBufferSize) {
        for (int i = 0; i < C74_GENPLUGIN::num_inputs(); i++) {
            if (m_InputBuffers[i]) delete m_InputBuffers[i];
            m_InputBuffers[i] = new t_sample[bufferSize];
        }
        for (int i = 0; i < C74_GENPLUGIN::num_outputs(); i++) {
            if (m_OutputBuffers[i]) delete m_OutputBuffers[i];
            m_OutputBuffers[i] = new t_sample[bufferSize];
        }
        
        m_CurrentBufferSize = bufferSize;
    }
}
