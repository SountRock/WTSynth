/*
  ==============================================================================

    WTOSC.cpp
    Created: 16 Aug 2023 3:16:27pm
    Author:  LKDat

  ==============================================================================
*/

#include "WTOSC.h"

WTOSC::WTOSC(std::vector<float> waveTable, double sampleRate) : waveTable{std::move(waveTable)}, sampleRate{ sampleRate }
{

}

WTOSC::~WTOSC()
{

}


void WTOSC::setFrequency(float frequency)
{
	indexIncrement = (frequency) * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
	//indexIncrementPlus = freq * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
	//indexIncrement += indexIncrementPlus;
}


float WTOSC::getSample()
{
	const auto sample = interpolateLinearly() * adsr.getNextSample() * volume;
	index += indexIncrement + indexIncrementPlus;
	index = std::fmod(index, static_cast<float>(waveTable.size()));
	return sample;
}

float WTOSC::interpolateLinearly()
{
	const auto truncatedIndex = static_cast<int>(index);
	const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(waveTable.size());

	const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
	const auto truncatedIndexWeight = 1.f - nextIndexWeight;

	return truncatedIndexWeight * waveTable[truncatedIndex] + nextIndexWeight * waveTable[nextIndex];
}

void WTOSC::stop()
{
	index = 0.f;
	indexIncrement = 0.f;
}

bool WTOSC::isPlaying()
{
	return indexIncrement != 0.f;
}

void WTOSC::setPlusFrequency(double freq)
{
	indexIncrementPlus = freq * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
}
