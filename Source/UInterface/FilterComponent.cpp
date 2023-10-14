/*
  ==============================================================================

    FilterComponent.cpp
    Created: 28 Jul 2023 12:10:50am
    Author:  LKDat

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeID, juce::String filterFreqID, juce::String filterResID, juce::String allVolumeID)
{
	//*!!!
	juce::StringArray choises{ "LowPass", "BandPass", "HighPass" };
	filterTypeSel.addItemList(choises, 1);
	addAndMakeVisible(filterTypeSel);
	
	attachFilterType = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeID, filterTypeSel);
	
	setSliderSize(cutOff, cutOffLabel, apvts, filterFreqID, filterFreqAttach);
	setSliderSize(res, resLabel, apvts, filterResID, filterResAttach);
	addAndMakeVisible(cutOff);
	addAndMakeVisible(res);

	allVolumeLabel.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
	allVolumeLabel.setFont(15.f);
	allVolumeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(allVolumeLabel);

	allVolume.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	allVolume.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 25);
	allVolume.setTextValueSuffix("%");
	allVolume.setSkewFactor(0.2);
	addAndMakeVisible(allVolume);

	allVolumeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, allVolumeID, allVolume);
	//*/
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{

}

void FilterComponent::resized()
{
	filterTypeSel.setBounds(0, 0, getWidth() / 3, getHeight() / 6);
	cutOff.setBounds(0, filterTypeSel.getHeight(), getWidth() / 2, getHeight() / 2);
	cutOffLabel.setBounds(cutOff.getX(), cutOff.getY() + cutOff.getHeight() + 10, cutOff.getWidth(), cutOff.getHeight() / 5);
	res.setBounds(cutOff.getWidth(), filterTypeSel.getHeight(), getWidth() / 2, getHeight() / 2);
	resLabel.setBounds(res.getX(), res.getY() + res.getHeight() + 10, res.getWidth(), res.getHeight() / 5);
	allVolumeLabel.setBounds(0, resLabel.getY() + resLabel.getHeight() + 10, getWidth()/5, resLabel.getHeight());
	allVolume.setBounds(allVolumeLabel.getX() + allVolumeLabel.getWidth() + 20, resLabel.getY() + resLabel.getHeight() + 10, getWidth() - allVolumeLabel.getWidth() - 20, resLabel.getHeight());
}
