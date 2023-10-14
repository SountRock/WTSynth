/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WTSynthAudioProcessorEditor::WTSynthAudioProcessorEditor (WTSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
	osc(audioProcessor.apvts, "OSCSTATUS1", "OSCMODOUTPUT1", "FMFREQ1", "FMDEPTH1", "OSCFREQ1", "VOLUME1", 1),
	osc2(audioProcessor.apvts, "OSCSTATUS2","OSCMODOUTPUT2", "FMFREQ2", "FMDEPTH2", "OSCFREQ2", "VOLUME2", 2),
	osc3(audioProcessor.apvts, "OSCSTATUS3", "OSCMODOUTPUT3", "FMFREQ3", "FMDEPTH3", "OSCFREQ3", "VOLUME3", 3),
	osc4(audioProcessor.apvts, "OSCSTATUS4", "OSCMODOUTPUT4", "FMFREQ4", "FMDEPTH4", "OSCFREQ4", "VOLUME4", 4),
	adsr(audioProcessor.apvts), 
	filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES", "VOLUME")
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(1000, 600);
	setResizable(true, true);

	juce::StringArray choises{ "Sine", "Saw", "Square" };
	juce::StringArray choisesOSC;
	choisesOSC.add("None");
	for (int i = 1; i <= osc.OSCCount; i++) {
		choisesOSC.add("OSC" + juce::String(i));
	}

	//createOSCs(OSCComp, audioProcessor.OSCILLATORS_COUNT);

	//OSCComponent osc1 = OSCComponent(audioProcessor.apvts, "OSCSTATUS1", "OSCMODOUTPUT1", "FMFREQ1", "FMDEPTH1", "OSCFREQ1", "VOLUME1", 1);
	//OSCComp.emplace_back(osc1);
	//OSCComp.add(osc1*);

	//////////////////////OSC1
	osc.setComboBoxChoise(osc.modOscSel, choisesOSC);
	//////////////////////OSC2
	osc2.setComboBoxChoise(osc2.modOscSel, choisesOSC);
	//////////////////////OSC3
	osc3.setComboBoxChoise(osc3.modOscSel, choisesOSC);
	//////////////////////OSC4
	osc4.setComboBoxChoise(osc4.modOscSel, choisesOSC);

	addAndMakeVisible(osc);
	addAndMakeVisible(osc2);
	addAndMakeVisible(osc3);
	addAndMakeVisible(osc4);
	addAndMakeVisible(adsr);
	addAndMakeVisible(filter);
}

WTSynthAudioProcessorEditor::~WTSynthAudioProcessorEditor()
{
}

//==============================================================================
void WTSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void WTSynthAudioProcessorEditor::resized()
{
	osc.setBounds(0, 0, getWidth() / 4, getHeight() / 2);
	osc2.setBounds(osc.getX() + getWidth() / 4, 0, getWidth() / 4, getHeight() / 2);
	osc3.setBounds(osc2.getX() + getWidth() / 4, 0, getWidth() / 4, getHeight() / 2);
	osc4.setBounds(osc3.getX() + getWidth() / 4, 0, getWidth() / 4, getHeight() / 2);
	adsr.setBounds(0, getHeight() / 2, getWidth() / 2, getHeight() / 2);
	filter.setBounds(getWidth() / 2, getHeight() / 2, getWidth() / 2, getHeight() / 2);
}
