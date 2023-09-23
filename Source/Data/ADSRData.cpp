/*
  ==============================================================================

    ADSRData.cpp
    Created: 13 Mar 2023 2:32:08pm
    Author:  LKDat

  ==============================================================================
*/

#include "ADSRData.h"

void ADSRData::updateADSR(const float attack, const float decay, const float sustain, const float realeaze)
{
	adsrParam.attack = attack;
	adsrParam.decay = decay;
	adsrParam.sustain = sustain;
	adsrParam.release = realeaze;

	setParameters(adsrParam);
}