/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UInterface//ADSRComponent.h"
#include "UInterface//FilterComponent.h"
#include "UInterface//OSCComponent.h"
//==============================================================================
/**
*/
class WTSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WTSynthAudioProcessorEditor (WTSynthAudioProcessor&);
    ~WTSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WTSynthAudioProcessor& audioProcessor;

	OSCComponent osc;
	OSCComponent osc2;
	OSCComponent osc3;
	OSCComponent osc4;

	//std::vector<OSCComponent> OSCComp;

	//juce::OwnedArray<OSCComponent*> OSCComp;

	ADSRComponent adsr;
	FilterComponent filter;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WTSynthAudioProcessorEditor)
};
