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
	void setENVLParams(double attack, double decay, double sustain, double realize);

private:
	void initializeOscillators();
	std::vector<float> generateSineWaveTable();
	void handleMidiEvent(const juce::MidiMessage& midiEvent);
	float midiNoteNumberToFrequency(int midiNoteNumber);
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);

	void noteOn(int timeInSamples);
	void noteOff(int timeInSamples);

	double sampleRate;
	std::vector<WTOSC> oscillators;

	int OSCILLATORS_COUNT{ 128 };

	float timeKoeff;
	float timeNote = 0;
	int timeInSamples = 0;

	int attackInSamples;
	int decayInSamples;
	int sustainInSamples;
	int realizeInSamples;

	double koeffAttack;
	double koeffDecay;
	double koeffSustain;
	double koeffRealize;

	int lastOscillatorID = 1;

	bool oscPlay = false;



	std::vector<double> envl;
};


