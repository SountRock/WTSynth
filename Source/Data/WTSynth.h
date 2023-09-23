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
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
	void setENVLParams(double attack, double decay, double sustain, double realize);

private:
	void initializeOscillators();
	std::vector<float> generateSineWaveTable();
	void handleMidiEvent(const juce::MidiMessage& midiEvent, juce::AudioBuffer<float>& buffer);
	float midiNoteNumberToFrequency(int midiNoteNumber);
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
	std::vector<float> renderSubbuffer() {
		for (auto& oscillator : oscillators) {

		}
	}


	void noteOn(int timeInSamples);
	void noteOff(int timeInSamples);

	double sampleRate;
	int numChannels = 2;
	std::vector<WTOSC> oscillators;

	//std::vector<WTOSC> oscillators2;

	//std::vector<std::vector<WTOSC>> oscillators2;

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

	int lastOscillatorID = -1;

	bool oscPlay = false;

	bool newNote = false;

	std::vector<float> subbuffer;
	std::vector<float> lastSubbuffer;
	//juce::AudioBuffer<float> subbuffer;
	//juce::AudioBuffer<float> lastSubbuffer;
	int lastTimeInSamples = 0;

	int phase = 0;

	float lastFreq = 0;
	float currentFreq = 0;

	int currentSample = 0;

	std::vector<double> envl;

	//ADSRData adsr;
};


