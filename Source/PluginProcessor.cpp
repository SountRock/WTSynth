/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "Data//WTSynth.h"

//==============================================================================
WTSynthAudioProcessor::WTSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
	
}

WTSynthAudioProcessor::~WTSynthAudioProcessor()
{
}

//==============================================================================
const juce::String WTSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WTSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WTSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WTSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WTSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WTSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WTSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WTSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WTSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void WTSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WTSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	synth.prepareToPlay(sampleRate);

	this->sampleRate = sampleRate;
}

void WTSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WTSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WTSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

	/*
	//Time On
	float interval = 1 / sampleRate;

	double time = (interval * timeInSamples * 1000) / buffer.getNumChannels();
	timeInSamples++;

	DBG(time);
	//Time On
	*/

	buffer.clear();

	///////////////////////////////////////////////////ENVELOPE
	//envlPhase = (int)(timePhase / (500 / sampleRate));
	//attackPhase = (int)(attackTime / (500 / sampleRate));
	//decayPhase = (int)(decayTime / (500 / sampleRate));
	//realizePhase = (int)(realizeTime / (500 / sampleRate));

	auto& attack = *apvts.getRawParameterValue("ATTACK");
	auto& decay = *apvts.getRawParameterValue("DECAY");
	auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
	auto& releaze = *apvts.getRawParameterValue("RELEAZE");

	synth.setENVLParams(attack, decay, sustain, releaze);


	/*
	if (envlChanged) {
		envl.swap(envlGenerate(buffer.getNumSamples(), 4.0, 1.0, 3.0, 1.0));
	}
	*/
	///////////////////////////////////////////////////ENVELOPE

	//попробывать сделать генерацию волны прараллельной обрабтке семплов. Сделать отдельный таймер в WTSynth который запускается при midiEvent.isNoteOn() и отключается midiEvent.isAllNotesOff()

	synth.processBlock(buffer, midiMessages);
	
}

//==============================================================================
bool WTSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WTSynthAudioProcessor::createEditor()
{
    return new WTSynthAudioProcessorEditor (*this);
}

//==============================================================================
void WTSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WTSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//ENVELOPE==============================================================================
/*
std::vector<double> WTSynthAudioProcessor::envlGenerate(int numSamples, double attack, double decay, double sustain, double realize)
{
	std::vector<double> envlGen;
	//envlGen.resize(numSamples);

	int attackPhase = attack * sampleRate * numSamples / 1000;
	int decayPhase = attackPhase + (decay * sampleRate * numSamples / 1000);
	int sustainPhase = decayPhase + (sustain * sampleRate * numSamples / 1000);
	int realizePhase = sustainPhase + (realize * sampleRate * numSamples / 1000);

	envlGen.resize(realizePhase);
	
	double koeffAttack = 1.0 / attackPhase;
	double koeffDecay = (1.0 - 0.7) / (decayPhase - attackPhase);
	double koeffSustain = 0.7;
	double koeffRealize = koeffSustain / (numSamples - sustainPhase);

	/////////GENERATE 
	//Attack generate
	for (auto a = 0; a < numSamples; a++) {
		envlGen[a] = koeffAttack * a;
	}
	//Decay generate
	for (auto d = attackPhase; d < decayPhase; d++) {
		envlGen[d] = 1.0 - koeffDecay * d;
	}
	//Sustain generate
	for (auto s = decayPhase; s < sustainPhase; s++) {
		envlGen[s] = koeffSustain * s;
	}
	//Realize generate
	for (auto r = sustainPhase; r < realizePhase; r++) {
		envlGen[r] = koeffSustain - koeffRealize * r;
	}
	/////////GENERATE 

	envlChanged = false;
	return envlGen;
}
*/
//ENVELOPE==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout WTSynthAudioProcessor::createParams()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	//////////////////////Volume 
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));

	//////////////////////ADSR
	params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{0.00001f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEAZE", "Releaze", juce::NormalisableRange<float>{0.1f, 4.0f, 0.001f}, 0.4f));

	//////////////////////ADSR ADVANCED
	///////ATTACK ADVANCED
	params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACKSCALE", "Attack SCALE", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	///////DECAY ADVANCED
	params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAYCALE", "Decay SCALE", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	///////SUSTAIN ADVANCED
	params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAINSCALE", "Sustain Scale", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	///////REALIZE ADVANCED
	params.push_back(std::make_unique<juce::AudioParameterFloat>("REALIZESCALE", "Realise Scale", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));

	///////POINTS LEVEL
	params.push_back(std::make_unique<juce::AudioParameterFloat>("ADPOINT", "AD Point", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("DSPOINT", "DS Point", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("SRPOINT", "SR Point", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	//
	// |        |AD |                  |
	// |       /|\  |                  |
	// |      / | \ |                  |
	// |     /  |  \|                  |SR 
	// |    /   |   |__________________|
	// |   /    |   |DS                |\
	// |  /     |   |                  | \
	// | /      |   |                  |  \
	// |/       |   |                  |   \
	//



	//////////////////////OSC1
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1STATUS", "Osc 1 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT1", "Osc 1 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT1", "Osc 1 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ1", "Osc 1 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH1", "Osc 1 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FREQ", "Osc 1 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME1", "Osc 1 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));
	//////////////////////OSC2
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2STATUS", "Osc 2 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT2", "Osc 2 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT2", "Osc 2 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc 2 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ2", "Osc 2 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH2", "Osc 2 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2FREQ", "Osc 2 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME2", "Osc 2 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));
	//////////////////////OSC3
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3STATUS", "Osc 3 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT3", "Osc 3 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT3", "Osc 3 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3WAVETYPE", "Osc 3 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ3", "Osc 3 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH3", "Osc 3 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3FREQ", "Osc 3 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME3", "Osc 3 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));
	//////////////////////OSC4
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC4STATUS", "Osc 4 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT4", "Osc 4 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT4", "Osc 4 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC4WAVETYPE", "Osc 4 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ4", "Osc 4 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH4", "Osc 4 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC4FREQ", "Osc 4 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME4", "Osc 4 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));

	//////////////////////filter
	params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "LowPass", "BandPass", "HighPass" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter CutOff", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>{1.0f, 10.0f, 0.1f}, 1.0f));
	//////////////////////filter ADSR 
	params.push_back(std::make_unique<juce::AudioParameterFloat>("MODATTACK", "Mod Attack", juce::NormalisableRange<float>{0.00001f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("MODDECAY", "Mod Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));

	return { params.begin(), params.end() };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WTSynthAudioProcessor();
}


