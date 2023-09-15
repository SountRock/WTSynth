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

	initializeOscillators(); //
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
	attackInSamples = (int) (attack / timeKoeff);
	decayInSamples = (int)(attackInSamples + decay / timeKoeff);
	sustainInSamples = (int)(decayInSamples + sustain / timeKoeff);
	realizeInSamples = (int)(sustainInSamples + realize / timeKoeff);
	
	koeffAttack = 1.0 / attackInSamples;
	koeffDecay = (1.0 - 0.7) / (decayInSamples - attackInSamples);
	koeffSustain = 0.7;
	koeffRealize = koeffSustain / (realizeInSamples - sustainInSamples);

	envl.resize(realizeInSamples);

	/*
	for (auto t = 0; t < envl.size(); t++) {
		if (t < attackInSamples) {
			envl[t] = koeffAttack * t;
			DBG("attack");
		}

		if (t >= attackInSamples && t < decayInSamples) {
			envl[t] = 1.0 - koeffDecay * (t - attackInSamples);
			DBG("decay");
		}

		if (t >= decayInSamples && t < sustainInSamples) {
			envl[t] = koeffSustain;
			DBG("sustain");
		}

		//if (t >= sustainInSamples && t < envl.size()) {
		if (t >= sustainInSamples) {
			envl[t] = koeffSustain - koeffRealize * (t - sustainInSamples);
			DBG("realize");
		}
	}
	*/
	
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
	const auto oscillatorID = midiEvent.getNoteNumber();
	if (midiEvent.isNoteOn()) {
		oscillators[lastOscillatorID].stop();
		timeInSamples = 0;
		//initializeOscillators();
		const auto freq = midiNoteNumberToFrequency(oscillatorID);
		oscillators[oscillatorID].setFrequency(freq);

		for (int s = 0; s < envl.size(); s++) {
			if (s < attackInSamples) {
				envl[s] = koeffAttack * s;
				DBG("attack");
			}

			if (s >= attackInSamples && s < decayInSamples) {
				envl[s] = 1.0 - koeffDecay * (s - attackInSamples);
				DBG("decay");
			}

			if (s >= decayInSamples && s < sustainInSamples) {
				envl[s] = koeffSustain;
				DBG("sustain");
			}

			if (s >= sustainInSamples && s < envl.size()) {
				envl[s] = koeffSustain - koeffRealize * (s - sustainInSamples);
				DBG("realize");
			}
		}

		oscillators[oscillatorID].setENVL(envl);

		lastOscillatorID = oscillatorID;
	//} else if (midiEvent.isNoteOff() && oscillators[oscillatorID].getTime() > sustainInSamples - 1) { //!!!
	} else if (midiEvent.isNoteOff()) {
		/////TIME
		//timeNote = 0;
		timeInSamples = sustainInSamples;
		/////TIME

		//oscillators[oscillatorID].getTime();
		//lastOscillatorID = midiEvent.getNoteNumber();
		//const auto oscillatorID = midiEvent.getNoteNumber();
		//oscillators[oscillatorID].stop();
		//oscillators.clear();

		DBG("off");
	} else if (midiEvent.isAllNotesOff()) {
		//for (auto& oscillator : oscillators) {
		//	oscillator.stop();
		//}
		//oscillators.clear();
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

	//*
	//timeInSamples = 0;
	for (auto& oscillator : oscillators) {
		if (oscillator.isPlaying()) {
			/////TIME
			timeInSamples++;

			if (timeInSamples == sustainInSamples - 1) {
				timeInSamples--;
			}
			/////TIME

			for (auto sample = startSample; sample < endSample; ++sample) {
				if (timeInSamples < envl.size()) {
					firstChannel[sample] += oscillator.getSample(timeInSamples);
				}
			}
		}
	}
	//*/
	
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