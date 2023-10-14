/*
  ==============================================================================

    FilterComponent.h
    Created: 28 Jul 2023 12:10:50am
    Author:  LKDat

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class FilterComponent  : public juce::Component
{
public:
	FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelID, juce::String fmFreqID, juce::String fmDepthID, juce::String allVolumeID);
	~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	using AttachSlider = juce::AudioProcessorValueTreeState::SliderAttachment;
	using AttachComboBox = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

	juce::ComboBox filterTypeSel;
	std::unique_ptr<AttachComboBox> attachFilterType;

	juce::Slider cutOff;
	juce::Slider res; 
	juce::Slider allVolume;

	std::unique_ptr<AttachSlider> filterFreqAttach;
	std::unique_ptr<AttachSlider> filterResAttach;
	std::unique_ptr<AttachSlider>  allVolumeAttach;

	juce::Label FilterTypeLabel{ "Filter Type", "Filter Type" };
	juce::Label cutOffLabel{ "CutOff", "CutOff" };
	juce::Label resLabel{ "Reso", "Reso" };
	juce::Label allVolumeLabel{ "Volume", "Volume" };
	

	void setSliderSize(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<AttachSlider>& attach) {
		slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 25);
		slider.setColour(juce::Slider::thumbColourId, juce::Colour(242, 184, 189));
		slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(184, 17, 64));
		addAndMakeVisible(slider);

		attach = std::make_unique<AttachSlider>(apvts, paramID, slider);

		label.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
		label.setFont(15.f);
		label.setJustificationType(juce::Justification::centred);
		addAndMakeVisible(label);
	};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
