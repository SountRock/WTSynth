/*
  ==============================================================================

	OSCComponent.cpp
	Created: 20 Mar 2023 8:31:44pm
	Author:  LKDat

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OSCComponent.h"

//==============================================================================
OSCComponent::OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String statusID, juce::String modOutID, juce::String fmFreqID, juce::String fmDepthID, juce::String oscFreqID, juce::String oscVolumeID, const int numberOsc)
{
	this->numberOsc = numberOsc;

	oscNumber.setText("Osc " + juce::String(numberOsc), juce::dontSendNotification);
	oscNumber.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
	oscNumber.setFont(15.f);
	oscNumber.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(oscNumber);

	/////////////////////////Status
	statusButton.setColour(juce::TextButton::buttonColourId, juce::Colour(101, 63, 201));
	statusButton.setColour(juce::TextButton::textColourOffId, juce::Colours::whitesmoke);
	statusButton.setButtonText("G&M");
	freqSlider.setEnabled(true);
	fmFreqSlider.setEnabled(true);
	fmDepthSlider.setEnabled(true);
	volume.setEnabled(true);
	addAndMakeVisible(statusButton);

	setSliderSize(freqSlider, juce::Slider::TextBoxAbove, freqLabel, apvts, oscFreqID, freqAttach, " Hz");
	freqAttach = std::make_unique<AttachSlider>(apvts, oscFreqID, freqSlider);
	setSliderSize(fmFreqSlider, juce::Slider::TextBoxBelow, fmFreqLabel, apvts, fmFreqID, fmFreqAttach, " Hz");
	setSliderSize(fmDepthSlider, juce::Slider::TextBoxBelow, fmDepthLabel, apvts, fmDepthID, fmDepthAttach, " dB");

	setStatus(apvts, statusID);
	lastNumberMod = std::to_string(numberOsc);

	statusButton.onClick = [&apvts, statusID, modOutID, this] {counterClickStatus++; setStatus(apvts, statusID); };
	addAndMakeVisible(modOscSel);
	modOscSel.onChange = [&apvts, modOutID, numberOsc, this] { setMod(apvts, modOutID); };

	/////////////////////////
	volume.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	volume.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 25);
	volume.setTextValueSuffix("%");
	volume.setSkewFactor(0.2f);
	addAndMakeVisible(volume);

	volumeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, oscVolumeID, volume);
	////////////////////////

	m_count()++;

}

OSCComponent::~OSCComponent()
{
	m_count()--;
}

void OSCComponent::paint(juce::Graphics& g)
{
	//g.drawText("Oscillator" + String(numberOsc), getLocalBounds(), juce::Justification::centred, true);
}

void OSCComponent::setStatus(juce::AudioProcessorValueTreeState& choise, juce::String statusID)
{
	if (counterClickStatus > 1) { counterClickStatus = 0; volume.setValue(lastVolume); }
	if (counterClickStatus < 0) { jassertfalse; }

	choise.getRawParameterValue(statusID)->store(counterClickStatus);

	if (counterClickStatus == 0) {
		statusButton.setColour(juce::TextButton::buttonColourId, juce::Colour(101, 63, 201));
		statusButton.setButtonText("G&M");
		freqSlider.setEnabled(true);
		fmFreqSlider.setEnabled(true);
		fmDepthSlider.setEnabled(true);
		volume.setEnabled(true);
	}
	if (counterClickStatus == 1) {
		statusButton.setColour(juce::TextButton::buttonColourId, juce::Colour(229, 43, 94));
		statusButton.setButtonText("MOD");
		freqSlider.setEnabled(false);
		fmFreqSlider.setEnabled(true);
		fmDepthSlider.setEnabled(true);
		volume.setEnabled(false);
		lastVolume = volume.getValue();
		choise.getRawParameterValue("VOLUME" + std::to_string((int)numberOsc))->store(0.0f);
		volume.setValue(0.0f);
	}

}

void OSCComponent::setMod(juce::AudioProcessorValueTreeState & choise, juce::String modOutID)
{
	if (modOscSel.getSelectedId() > 0) {
		choise.getRawParameterValue(modOutID)->store(modOscSel.getSelectedId() - 1);
	}

	if ((int)* choise.getRawParameterValue(modOutID) > 0) {
		std::string numberMod = std::to_string((int)* choise.getRawParameterValue(modOutID));
		fmFreqAttach = std::make_unique<AttachSlider>(choise, ("FMFREQ" + numberMod), fmFreqSlider);
		fmDepthAttach = std::make_unique<AttachSlider>(choise, ("FMDEPTH" + numberMod), fmDepthSlider);
		lastNumberMod = numberMod;
		fmFreqSlider.setValue(0.0f);
		fmDepthSlider.setValue(0.0f);
	}
	else {
		fmFreqSlider.setValue(0.0f);
		fmDepthSlider.setValue(0.0f);
		fmFreqAttach = 0;
		fmDepthAttach = 0;
		choise.getRawParameterValue("FMFREQ" + lastNumberMod)->store(0.0f);
		choise.getRawParameterValue("FMDEPTH" + lastNumberMod)->store(0.0f);
	}
}

int OSCComponent::getNumberOsc()
{
	return numberOsc;
}

int OSCComponent::getStatus()
{
	return counterClickStatus;
}

void OSCComponent::resized()
{
	oscNumber.setBounds(0, 0, getWidth(), getHeight() * 0.0625);
	statusButton.setBounds(oscNumber.getX() + oscNumber.getWidth(), 0, getWidth() / 2, getHeight() * 0.0625);
	modOscSel.setBounds(0, oscNumber.getY() + oscNumber.getHeight(), getWidth(), getHeight() * 0.0625);

	freqLabel.setBounds(getWidth() / 4, modOscSel.getY() + modOscSel.getHeight() + 10, getWidth() / 2, getHeight() * 0.0625);
	freqSlider.setBounds(getWidth() / 4, freqLabel.getY() + freqLabel.getHeight(), getWidth() / 2, getHeight() * 0.25);

	fmFreqSlider.setBounds(0, freqSlider.getY() + freqSlider.getHeight(), getWidth() / 2, getHeight() * 0.25);
	fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() + fmFreqSlider.getHeight(), fmFreqSlider.getWidth(), getHeight() * 0.0625);

	fmDepthSlider.setBounds(fmFreqSlider.getWidth(), freqSlider.getY() + freqSlider.getHeight(), getWidth() / 2, getHeight() * 0.25);
	fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() + fmDepthSlider.getHeight(), fmDepthSlider.getWidth(), getHeight() * 0.0625);

	volume.setBounds(10, fmDepthLabel.getY() + fmDepthLabel.getHeight() + 10, getWidth() - 20, getHeight() * 0.0625);
}