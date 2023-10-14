/*
  ==============================================================================

    ADSRData.h
    Created: 13 Mar 2023 2:32:08pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ADSRData : public juce::ADSR {
public:

	void updateADSR(const float attack, const float decay, const float sustain, const float realeaze);

private:
	juce::ADSR::Parameters adsrParam;
};
