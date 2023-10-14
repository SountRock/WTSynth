/*
  ==============================================================================

	OSCComponent.h
	Created: 20 Mar 2023 8:31:44pm
	Author:  LKDat

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class OSCComponent : public juce::Component
{
public:
	OSCComponent() {};
	OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String statusID, juce::String modOutID, juce::String fmFreqID, juce::String fmDepthID, juce::String oscFreqID, juce::String oscVolumeID, const int numberOsc);
	~OSCComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	int getNumberOsc();

	const size_t& OSCCount = m_count();

	juce::ComboBox oscWaveSel;
	juce::ComboBox modOscSel;


	void setComboBoxChoise(juce::ComboBox& comboBox, juce::StringArray choises) {
		comboBox.addItemList(choises, 1);
		comboBox.setSelectedId(1);
	};


	int getStatus();

private:

	std::string lastNumberMod;

	int numberOsc = 0;

	int counterClickStatus = 0;

	float lastVolume;

	//
	size_t& m_count()
	{
		static size_t c(0);
		return c;
	}
	//

	void setStatus(juce::AudioProcessorValueTreeState& choise, juce::String statusID);
	void setMod(juce::AudioProcessorValueTreeState& choise, juce::String modSelID);

	juce::Label oscNumber;

	using AttachSlider = juce::AudioProcessorValueTreeState::SliderAttachment;
	using AttachComboBox = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
	using ButtonAttachment =juce::AudioProcessorValueTreeState::ButtonAttachment;

	juce::TextButton statusButton;
	juce::Slider freqSlider;
	juce::Slider fmFreqSlider;
	juce::Slider fmDepthSlider;
	juce::Slider volume;

	std::unique_ptr<ButtonAttachment> statusAttach;
	std::unique_ptr<AttachSlider> freqAttach;
	std::unique_ptr<AttachSlider> fmFreqAttach;
	std::unique_ptr<AttachSlider> fmDepthAttach;
	std::unique_ptr<AttachSlider> volumeAttach;
	std::unique_ptr<AttachComboBox> attachModOscSel;

	juce::Label freqLabel{ "Freq", "Freq" };
	juce::Label fmFreqLabel{ "FM Freq", "FM Freq" };
	juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };

	void setSliderSize(juce::Slider& slider, juce::Slider::TextEntryBoxPosition textBoxPosition, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<AttachSlider>& attach, juce::String valueSuffix) {
		slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		slider.setTextBoxStyle(textBoxPosition, false, 70, 25);
		slider.setTextValueSuffix(valueSuffix);
		slider.setColour(juce::Slider::thumbColourId, juce::Colour(242, 184, 189));
		slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(184, 17, 64));
		slider.setNormalisableRange(juce::NormalisableRange<double> {0.0f, 1000.0f, 0.01f, 0.3f});
		addAndMakeVisible(slider);

		label.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
		label.setFont(15.f);
		label.setJustificationType(juce::Justification::centred);
		addAndMakeVisible(label);
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCComponent)
};