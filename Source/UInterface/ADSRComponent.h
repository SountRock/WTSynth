/*
  ==============================================================================

    ADSRComponent.h
    Created: 13 Mar 2023 2:29:14pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
	
private:
	void ADSRComponent::setSliderParams(juce::Slider& slider);

	juce::Slider attackSlider;
	juce::Slider decaySlider;
	juce::Slider sustainSlider;
	juce::Slider releazeSlider;

	juce::Label attackLabel{ "attackLabel", "A" };
	juce::Label decayLabel{ "decayLabel", "D" };
	juce::Label sustainLabel{ "sustainLabel", "S" };
	juce::Label releazeLabel{ "releaseLabel", "R" };

	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	std::unique_ptr<SliderAttachment> attachAttack;
	std::unique_ptr<SliderAttachment> attachDecay;
	std::unique_ptr<SliderAttachment> attachSustain;
	std::unique_ptr<SliderAttachment> attachReleaze;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
