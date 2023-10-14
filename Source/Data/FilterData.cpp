/*
  ==============================================================================

    FilterData.cpp
    Created: 26 Jul 2023 11:03:47pm
    Author:  LKDat

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
	filter.reset();

	juce::dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, numChannels };
	filter.prepare(spec);

	isPrepared = true;
}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{
	jassert(isPrepared);

	juce::dsp::AudioBlock<float> block{buffer};
	filter.process(juce::dsp::ProcessContextReplacing<float> { block });
}

void FilterData::updateParams(const int filterType, const float resonance, const float frequency)
{
	
	switch (filterType)
	{
	case 0:
		filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
	case 1:
		filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
	case 2:
		filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
	}
	
	filter.setResonance(resonance);
	filter.setCutoffFrequency(frequency);

}

void FilterData::reset()
{
	filter.reset();
}
