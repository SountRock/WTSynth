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
	float getSample(double level);
	float interpolateLinearly();
	void stop();
	bool isPlaying();

	//void setENVL(std::vector<double> envl) { this->envl.swap(envl); };

private:
	std::vector<float> waveTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;

	//std::vector<double> envl;
};
