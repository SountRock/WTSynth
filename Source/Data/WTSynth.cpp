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
}

void WTSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	auto currentSample = 0;

	/////TIME
	timeKoeff = 1000 / (sampleRate * buffer.getNumChannels());
	/////TIME

	for (const auto midiMessage : midiMessages) {
		const auto midiEvent = midiMessage.getMessage();
		const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

		render(buffer, currentSample, midiEventSample);
		handleMidiEvent(midiEvent);

		currentSample = midiEventSample;
	}
	render(buffer, currentSample, buffer.getNumSamples());
}

void WTSynth::setENVLParams(double attack, double decay, double sustain, double realize)
{
	attackInSamples = attack / timeKoeff;
	decayInSamples = attackInSamples + decay / timeKoeff;
	sustainInSamples = decayInSamples + sustain / timeKoeff;
	realizeInSamples = sustainInSamples + realize / timeKoeff;
	
	koeffAttack = 1.0 / attackInSamples;
	koeffDecay = (1.0 - 0.7) / (decayInSamples - attackInSamples);
	koeffSustain = 0.7;
	koeffRealize = koeffSustain / (realizeInSamples - sustainInSamples);

	envl.resize(realizeInSamples);
}

void WTSynth::initializeOscillators()
{
	//constexpr auto OSCILLATORS_COUNT = 128;
	
	const auto waveTable = generateSineWaveTable();

	oscillators.clear();

	for (auto i = 0; i < OSCILLATORS_COUNT; ++i) {
		oscillators.emplace_back(waveTable, sampleRate);
	}
}

////////////////////////////////////////////////////////////////////////////////////
void WTSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn()) {
		const auto oscillatorID = midiEvent.getNoteNumber();
		const auto freq = midiNoteNumberToFrequency(oscillatorID);
		oscillators[oscillatorID].setFrequency(freq);
	//} else if (timeInSamples > envl.size()) {
	} else if (midiEvent.isNoteOff()) {
		/////TIME
		timeNote = 0;
		timeInSamples = 0;
		/////TIME
		
		//lastOscillatorID = midiEvent.getNoteNumber();
		const auto oscillatorID = midiEvent.getNoteNumber();
		oscillators[oscillatorID].stop();
	} else if (midiEvent.isAllNotesOff()) {
		for (auto& oscillator : oscillators) {
			oscillator.stop();
		}
	}
}

void WTSynth::noteOn(int timeInSamples)
{
	

	//if (timeInSamples >= sustainInSamples) { timeInSamples--; }
}

void WTSynth::noteOff(int timeInSamples)
{
	
}

////////////////////////////////////////////////////////////////////////////////////

float WTSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
	constexpr auto A4_FREQUENCY = 440.f;
	constexpr auto A4_NOTE_NUMBER = 69.f;
	constexpr auto SEMITONES_IN_AN_OCTAVE = 12.f;

	return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER) / SEMITONES_IN_AN_OCTAVE);
}

void WTSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
	auto* firstChannel = buffer.getWritePointer(0);

	for (auto& oscillator : oscillators) {
		if (oscillator.isPlaying()) {
			/////TIME
			timeInSamples++;
			/////TIME

			if (timeInSamples < attackInSamples) {
				envl[timeInSamples] = koeffAttack * timeInSamples;
				DBG("attack");
			}

			if (timeInSamples >= attackInSamples && timeInSamples < decayInSamples) {
				envl[timeInSamples] = 1.0 - koeffDecay * (timeInSamples - attackInSamples);
				DBG("decay");
			}

			if (timeInSamples >= decayInSamples && timeInSamples < sustainInSamples) {
				envl[timeInSamples] = koeffSustain;
				DBG("sustain");
			}

			if (timeInSamples >= sustainInSamples && timeInSamples < envl.size()) {
				envl[timeInSamples] = koeffSustain - koeffRealize * (timeInSamples - sustainInSamples);
				DBG("realize");
			}
			
			/////GENERATE 
			for (auto sample = startSample; sample < endSample; ++sample) {
				if (timeInSamples < envl.size()) {
					firstChannel[sample] += oscillator.getSample(envl[timeInSamples]);
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