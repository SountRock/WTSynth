/*
  ==============================================================================

    WT.h
    Created: 16 Aug 2023 2:31:13pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WTOSC.h"

class WTSynth
{
public:
	WTSynth();
	~WTSynth();

	void prepareToPlay(double sampleRate);
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

private:
	void initializeOscillators();
	std::vector<float> generateSineWaveTable();
	void handleMidiEvent(const juce::MidiMessage& midiEvent);
	float midiNoteNumberToFrequency(int midiNoteNumber);
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);

	double sampleRate;
	std::vector<WTOSC> oscillators;

	int OSCILLATORS_COUNT{ 128 };
};


