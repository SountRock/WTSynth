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

	//adsr.setSampleRate(sampleRate);//****
}

void WTSynth::releaseResorces()
{
	//oscillators.clear();
}

void WTSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	auto currentSample = 0;
	//currentSample = 0;

	/////TIME
	timeKoeff = 1000 / (sampleRate * buffer.getNumChannels());
	/////TIME

	numChannels = buffer.getNumChannels();

	for (const auto midiMessage : midiMessages) {
		const auto midiEvent = midiMessage.getMessage();
		const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

		handleMidiEvent(midiEvent, buffer);
		render(buffer, currentSample, midiEventSample);

		currentSample = midiEventSample;
		//adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples()); //****
	}
	render(buffer, currentSample, buffer.getNumSamples());
}
	

void WTSynth::setENVLParams(double attack, double decay, double sustain, double realize)
{
	attackInSamples = (int)(attack / timeKoeff);
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

	//////////////////
	subbuffer.resize(envl.size());
	lastSubbuffer.resize(envl.size());
	//////////////////
	
	//adsr.updateADSR(attack, decay, sustain, realize);//****
}

void WTSynth::initializeOscillators()
{
	//constexpr auto OSCILLATORS_COUNT = 128;
	
	const auto waveTable = generateSineWaveTable();

	oscillators.clear();
	//oscillators2.clear();/////////

	for (auto i = 0; i < OSCILLATORS_COUNT; ++i) {
		oscillators.emplace_back(waveTable, sampleRate);
		//oscillators2.emplace_back(waveTable, sampleRate);/////////
	}
}

////////////////////////////////////////////////////////////////////////////////////
void WTSynth::handleMidiEvent(const juce::MidiMessage& midiEvent, juce::AudioBuffer<float>& buffer)
{
	const auto oscillatorID = midiEvent.getNoteNumber();
	if (midiEvent.isNoteOn()) {
		oscPlay = true;
		//initializeOscillators();
		buffer.clear();
		const auto freq = midiNoteNumberToFrequency(oscillatorID);
		//oscillators[oscillatorID].setFrequency(lastFreq);
		//oscillators[lastOscillatorID].setFrequency(freq);
		oscillators[oscillatorID].setFrequency(freq);/////////

		currentFreq = freq;
		/*
		//buffer.clear(); /////////////
		/////////////
		auto* lastBuffChannel = lastSubbuffer.getWritePointer(0);
		//for (auto ch = 0; ch < buffer.getNumChannels(); ch++) {
		for (auto t = 0; t < lastSubbuffer.getNumSamples() - timeInSamples; ++t) {
				//lastSubbuffer.setSample(ch, t, oscillators[oscillatorID].getSample(t));
			lastBuffChannel[t] += oscillators[lastOscillatorID].getSample(t);
		}
		//}
		/////////////
		//oscillators[lastOscillatorID].stop();
		timeInSamples = 0;
		//initializeOscillators();
		*/

		envl[0] = 0.1;
		for (int s = 1; s < envl.size(); s++) {
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
		//oscillators2[oscillatorID].setENVL(envl);/////////

		//oscillators[oscillatorID].setPhase(timeInSamples);

		

		/*
		oscillators[lastOscillatorID].setENVL(envl);
		if (lastOscillatorID != oscillatorID) {
			for (auto t = 0; t < lastSubbuffer.size() - timeInSamples; t++) {
				lastSubbuffer[t] = oscillators[lastOscillatorID].getSample(t);
			}

			//oscillators[lastOscillatorID].stop();
			//oscillators[lastOscillatorID].setFrequency(lastFreq);
		}
		*/

		/*
		if (lastOscillatorID != oscillatorID) {
			//oscillators[lastOscillatorID].setPhase(phase);
			//oscillators[lastOscillatorID].stop();
			lastSubbuffer.clear();
			lastSubbuffer.resize(envl.size());
			for (auto t = 0; t < lastSubbuffer.size() - timeInSamples; t++)
			{
				lastSubbuffer[t] = t;
			}
		}
		*/

		if (lastOscillatorID < 0) {
			lastOscillatorID = oscillatorID;
		}

		if (lastOscillatorID != oscillatorID) {
			newNote = true;
			oscillators[lastOscillatorID].setPhase(timeInSamples);

			//for (auto s = 0; s < (envl.size() - oscillators[lastOscillatorID].getPhase()); s++) {
				//subbuffer[s] = oscillators[lastOscillatorID].getSample(s + oscillators[lastOscillatorID].getPhase());
			//}
		} else {
			newNote = false;
		}

		//oscillators[oscillatorID].phaseDestroy();
		//oscillators[lastOscillatorID].setPhase(phase);
		//oscillators[lastOscillatorID].setFrequency(lastFreq);

		timeInSamples = 0;

		//oscillators[lastOscillatorID].stop();
		//initializeOscillators();
		//lastOscillatorID = oscillatorID;
	//} else if (midiEvent.isNoteOff() && oscillators[oscillatorID].getTime() > sustainInSamples - 1) { //!!!
		//const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
		//render(buffer, currentSample, midiEventSample);
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

		lastFreq = midiNoteNumberToFrequency(oscillatorID);
		lastOscillatorID = oscillatorID;

		oscPlay = false;
		
		//DBG("off");
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
	//auto* buffChannel = subbuffer.getWritePointer(0);
	//auto* lastBuffChannel = lastSubbuffer.getReadPointer(0);

	//*
	//timeInSamples = 0;
	//for (auto& oscillator : oscillators) {
	for (auto o = 0; o < oscillators.size(); o++) {
		//if (oscillator.isPlaying()) {
		if (oscillators[o].isPlaying()) {
			/////TIME
			
			/*
			if (oscPlay) {
				DBG("resrender");
				render(buffer, startSample, endSample);
				oscPlay = false;
			}
			*/

			timeInSamples++;

			if (newNote) {
				DBG("resrender");
				o = lastOscillatorID;
				newNote = false;
			}

			if (oscPlay && o != lastOscillatorID) {
				timeInSamples == sustainInSamples;
			}
			
			/////TIME

			if (timeInSamples == realizeInSamples) {
				//oscillator.phaseDestroy();
				//oscillators[o].phaseDestroy();
				initializeOscillators();
				lastOscillatorID = -1;
			}
			
			//DBG(oscillator.getPhase());

			//oscillator.setFrequency(lastFreq);


			buffer.clear();
			//oscillator.setFrequency(currentFreq);
			
			for (auto sample = startSample; sample < endSample; ++sample) {
				//if (timeInSamples + oscillator.getPhase() < envl.size()) {
				if (timeInSamples + oscillators[o].getPhase() < envl.size()) {
					//float last = oscillator.getSample(lastSubbuffer[timeInSamples]);
					//firstChannel[sample] += (oscillator.getSample(timeInSamples) + subbuffer[sample]);

					//firstChannel[sample] += oscillator.getSample(timeInSamples + oscillator.getPhase());

					firstChannel[sample] += oscillators[o].getSample(timeInSamples + oscillators[o].getPhase());

					//firstChannel[sample] += (oscillators[o].getSample(timeInSamples) + subbuffer[timeInSamples]);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	for (auto& oscillator2 : oscillators2) {
		if (oscillator2.isPlaying()) {
			/////TIME
			timeInSamples++;

			if (timeInSamples == sustainInSamples - 1) {
				timeInSamples--;
			}
			/////TIME

			if (timeInSamples == realizeInSamples) {
				phase = 0.0;
				initializeOscillators();
			}

			DBG(phase);

			//oscillator.setFrequency(lastFreq);

			buffer.clear();
			//oscillator.setFrequency(currentFreq);

			for (auto sample = startSample; sample < endSample; ++sample) {
				if (timeInSamples + phase < envl.size()) {
					//float last = oscillator.getSample(lastSubbuffer[timeInSamples]);
					firstChannel[sample] += oscillator2.getSample(phase + timeInSamples);
				}
			}
		}
	}
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
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