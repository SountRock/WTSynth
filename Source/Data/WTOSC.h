/*
  ==============================================================================

    WTOSC.h
    Created: 16 Aug 2023 3:16:27pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WTOSC {
public:
	WTOSC(std::vector<float> waveTable, double sampleRate);
	~WTOSC();

	void setFrequency(float frequency);
	float getSample(int time);
	float interpolateLinearly();
	void stop();
	bool isPlaying();

	void setENVL(std::vector<double> envl) { this->envl.swap(envl); };

	int getTime() { return time; };




	void setPhase(int phase) { this->phase = phase; };
	int getPhase() { return phase; };
	void phaseDestroy() { phase = 0; }; //////

private:
	std::vector<float> waveTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;

	std::vector<double> envl;
	int time = 0;

	bool phaseChanged = false; //////
	int phase = 0;
};
