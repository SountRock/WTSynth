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
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, double attack, double decay, double sustain, double realize, std::vector<double> volume, std::vector<double> freq, std::vector<double> fmDepth);
	void releaseResorces();

	int OSCILLATORS_COUNT{ 4 };

private:
	void initializeOscillators();
	std::vector<float> generateSineWaveTable();
	void handleMidiEvent(const juce::MidiMessage& midiEvent);
	float midiNoteNumberToFrequency(int midiNoteNumber);
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample, double attack, double decay, double sustain, double realize, std::vector<double> volume, std::vector<double> freq, std::vector<double> fmDepth);

	double sampleRate;
	int numChannels;
	float timeKoeff;
	int timeInSamples;
	std::vector<std::vector<WTOSC>> OSC;

	int OSCILLATORS_NOTE_COUNT{ 128 };
};


