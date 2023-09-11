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
	float getSample();
	float interpolateLinearly();
	void stop();
	bool isPlaying();

private:
	std::vector<float> waveTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;
};
