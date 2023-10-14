/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 13 Mar 2023 2:29:14pm
    Author:  LKDat

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState& apvts)
{
	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	attachAttack = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackSlider);
	attachDecay = std::make_unique<SliderAttachment>(apvts, "DECAY", decaySlider);
	attachSustain = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
	attachReleaze = std::make_unique<SliderAttachment>(apvts, "RELEAZE", releazeSlider);

	setSliderParams(attackSlider);
	setSliderParams(decaySlider);
	setSliderParams(sustainSlider);
	setSliderParams(releazeSlider);

	attackLabel.attachToComponent(&attackSlider, false);
	decayLabel.attachToComponent(&decaySlider, false);
	sustainLabel.attachToComponent(&sustainSlider, false);
	releazeLabel.attachToComponent(&releazeSlider, false);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
    //g.fillAll (juce::Colours::black);   
}

void ADSRComponent::resized()
{
	const auto bounds = getLocalBounds().reduced(10);
	const auto padding = 10;
	const auto sliderWith = bounds.getWidth() / 4 - padding;
	const auto sliderHeight = bounds.getHeight() - padding;
	const auto sliderStartX = 0;
	const auto sliderStartY = 0;

	attackSlider.setBounds(sliderStartX, sliderStartY, sliderWith, sliderHeight);
	decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWith, sliderHeight);
	decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWith, sliderHeight);
	sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWith, sliderHeight);
	releazeSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWith, sliderHeight);
}

void ADSRComponent::setSliderParams(juce::Slider& slider)
{
	slider.setSliderStyle(juce::Slider::LinearVertical);
	slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
	addAndMakeVisible(slider);
}
