/*
  ==============================================================================

    WT.cpp
    Created: 16 Aug 2023 2:31:13pm
    Author:  LKDat

  ==============================================================================
*/

#include "WTSynth.h"

WTSynth::WTSynth()
{
}

WTSynth::~WTSynth()
{
}

void WTSynth::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;

	initializeOscillators(); 

	for (auto o = 0; o < OSCILLATORS_COUNT; o++) {
		for (auto n = 0; n < OSC[o].size(); n++) {
			OSC[o][n].setSampleRateADSR(sampleRate);
		}
	}
}

void WTSynth::releaseResorces()
{
	for (auto o = 0; o < OSCILLATORS_COUNT; o++) {
		OSC[o].clear();
	}
}

void WTSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, double attack, double decay, double sustain, double realize, std::vector<double> volume, std::vector<double> freq, std::vector<double> fmDepth)
{
	auto currentSample = 0;

	/////TIME
	timeKoeff = 1000 / (sampleRate * buffer.getNumChannels());
	/////TIME

	numChannels = buffer.getNumChannels();

	for (const auto midiMessage : midiMessages) {
		const auto midiEvent = midiMessage.getMessage();
		const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

		handleMidiEvent(midiEvent);
		render(buffer, currentSample, midiEventSample, attack, decay, sustain, realize, volume, freq, fmDepth);

		currentSample = midiEventSample;
	}
	render(buffer, currentSample, buffer.getNumSamples(), attack, decay, sustain, realize, volume, freq, fmDepth);
}

void WTSynth::initializeOscillators()
{
	const auto waveTable = generateSineWaveTable();
	OSC.resize(OSCILLATORS_COUNT);
	for (auto o = 0; o < OSCILLATORS_COUNT; o++) {
		OSC[o].clear();
		for (auto n = 0; n < OSCILLATORS_NOTE_COUNT; ++n) {
			OSC[o].emplace_back(waveTable, sampleRate);;
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////
void WTSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	const auto oscillatorID = midiEvent.getNoteNumber();
	if (midiEvent.isNoteOn()) {
		const auto freq = midiNoteNumberToFrequency(oscillatorID);
		for (auto o = 0; o < OSCILLATORS_COUNT; o++) {
			OSC[o][oscillatorID].setFrequency(freq);
			OSC[o][oscillatorID].noteOn();
		}

		timeInSamples = 0;
	} else if (midiEvent.isNoteOff()) {
		for (auto o = 0; o < OSCILLATORS_COUNT; o++) {
			OSC[o][oscillatorID].noteOff();
		}

	} else if (midiEvent.isAllNotesOff()) {
		for (auto o = 0; o < OSCILLATORS_COUNT; o++) {
			for (auto& oscillator : OSC[o]) {
				oscillator.stop();
			}
		}

	} 
}

////////////////////////////////////////////////////////////////////////////////////

float WTSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
	constexpr auto A4_FREQUENCY = 440.f;
	constexpr auto A4_NOTE_NUMBER = 69.f;
	constexpr auto SEMITONES_IN_AN_OCTAVE = 12.f;

	return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER) / SEMITONES_IN_AN_OCTAVE);
}

void WTSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample, double attack, double decay, double sustain, double realize, std::vector<double> volume, std::vector<double> freq, std::vector<double> fmDepth)
{
	auto* firstChannel = buffer.getWritePointer(0);

	for (auto o = 0; o < OSCILLATORS_COUNT; o++) {
		for (auto n = 0; n < OSC[o].size(); n++) {
			if (OSC[o][n].isPlaying()) {
				OSC[o][n].updateADSR(attack, decay, sustain, realize);
				OSC[o][n].setVolume(volume[o]); 
				OSC[o][n].setPlusFrequency(freq[o]);
				for (auto sample = startSample; sample < endSample; ++sample) {
					firstChannel[sample] += OSC[o][n].getSample();
				}
			}
		}
	}
	
	for (auto channel = 1; channel < buffer.getNumChannels(); ++channel) {
		std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
	}
}

std::vector<float> WTSynth::generateSineWaveTable()
{
	constexpr auto WAVETABLE_LENGTH = 64;

	std::vector<float> sineWaveTable(WAVETABLE_LENGTH);

	const auto PI = std::atanf(1.f) * 4;

	for (auto i = 0; i < WAVETABLE_LENGTH; ++i) {
		sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / static_cast<float>(WAVETABLE_LENGTH));
	}

	return sineWaveTable;
}