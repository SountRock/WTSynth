/*
  ==============================================================================

    FilterData.h
    Created: 26 Jul 2023 11:03:47pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FilterData {
public:
	void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
	void process(juce::AudioBuffer<float>& buffer);
	void updateParams(const int filterType, const float resonance, const float frequency);
	void reset();

private:
	juce::dsp::StateVariableTPTFilter<float> filter;
	bool isPrepared{ false };
};
