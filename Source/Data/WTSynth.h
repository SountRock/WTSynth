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
#include "ADSRData.h"

class WTSynth
{
public:
	WTSynth();
	~WTSynth();

	void prepareToPlay(double sampleRate);
	void releaseResorces();
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, double attack, double decay, double sustain, double realize, std::vector<double> volume,  std::vector<double> freq, std::vector<double> fmDepth);


	int OSCILLATORS_COUNT{ 4 };
private:
	void initializeOscillators();
	std::vector<float> generateSineWaveTable();
	void handleMidiEvent(const juce::MidiMessage& midiEvent);
	float midiNoteNumberToFrequency(int midiNoteNumber);
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample, double attack, double decay, double sustain, double realize, std::vector<double> volume, std::vector<double> freq, std::vector<double> fmDepth);

	double sampleRate;
	int numChannels = 2;
	std::vector<WTOSC> oscillators;
	std::vector<WTOSC> oscillators2;
	std::vector<WTOSC> oscillators3;
	std::vector<WTOSC> oscillators4;

	std::vector<std::vector<WTOSC>> OSC;

	int OSCILLATORS_NOTE_COUNT{ 128 };

	float timeKoeff;
	float timeNote = 0;
	int timeInSamples = 0;

	int lastTimeInSamples = 0;

	int currentSample = 0;
	ADSRData adsr;
	std::vector<double> freqOSC;
	int lastOscillatorID = 0;
};


