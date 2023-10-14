/*
  ==============================================================================

    WTOSC.h
    Created: 16 Aug 2023 3:16:27pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ADSRData.h"

class WTOSC {
public:
	WTOSC(std::vector<float> waveTable, double sampleRate);
	~WTOSC();

	void setFrequency(float frequency);
	float getSample();
	float interpolateLinearly();
	void stop();
	bool isPlaying();

	int getTime() { return time; };

	void noteOn() { adsr.noteOn(); };
	void noteOff() { adsr.noteOff(); };
	void setSampleRateADSR(double sampleRate) { adsr.setSampleRate(sampleRate); };
	void updateADSR(double attack, double decay, double sustain, double realize) { adsr.updateADSR(attack, decay, sustain, realize); };
	void setPlusFrequency(double freq);
	void setVolume(double volume) { this->volume = volume; };

	void setModulation(std::vector<float> modTable, float indexIncrementMod) { this->modTable = modTable; this->indexIncrementMod = indexIncrementMod; };
	void killModulation() { modTable.clear();  indexIncrementMod = 0.f; };
	std::vector<float> getTable() { return waveTable; };
	void setFMDepth(float fmDepth) { this->fmDepth = fmDepth; };
	float getPlusIncrement() { return indexIncrementPlus; };

private:
	std::vector<float> waveTable;
	std::vector<float> modTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;
	float indexIncrementPlus = 0.f;
	float indexIncrementMod = 0.f;

	ADSRData adsr;
	int time = 0;

	double volume = 0.5;
	double plusFreq = 0.0;
	double frequency = 0.0;
	double fmDepth = 0.0;
};
