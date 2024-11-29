#pragma once

#include <vector>
#include <juce_audio_processors/juce_audio_processors.h>

#include "tools/SynchronBlockProcessor.h"
#include "tools/AudioProcessParameter.h"
#include "PluginSettings.h"
#include "FormantFilterAlgo.h"
#include "XYComponent.h"

class FormantFilterAudioProcessor;

// This is how we define our parameter as globals to use it in the audio processor as well as in the editor
const struct
{
	const std::string ID = "F1FrequencyID";
	const std::string name = "F1Frequency";
	const std::string unitName = " Hz";
	const float minValue = 200.f;
	const float maxValue = 950.f;
	const float defaultValue = 600.f;
}g_paramF1Frequency;

const struct
{
	const std::string ID = "F2FrequencyID";
	const std::string name = "F2Frequency";
	const std::string unitName = " Hz";
	const float minValue = 600.f;
	const float maxValue = 3000.f;
	const float defaultValue = 1600.f;
}g_paramF2Frequency;

const struct
{
	const std::string ID = "F1RadiusID";
	const std::string name = "F1Radius";
	const std::string unitName = "";
	const float minValue = 0.f;
	const float maxValue = 0.99f;
	const float defaultValue = 0.7f;
}g_paramF1Radius;

const struct
{
	const std::string ID = "F2RadiusID";
	const std::string name = "F2Radius";
	const std::string unitName = "";
	const float minValue = 0.f;
	const float maxValue = 0.99f;
	const float defaultValue = 0.7f;
}g_paramF2Radius;


class FormantFilterAudio : public SynchronBlockProcessor
{
public:
    FormantFilterAudio(FormantFilterAudioProcessor* processor);
    void prepareToPlay(double sampleRate, int max_samplesPerBlock, int max_channels);
 	virtual int processSynchronBlock(juce::AudioBuffer<float>&, juce::MidiBuffer& midiMessages, int NrOfBlocksSinceLastProcessBlock);    

    // parameter handling
  	void addParameter(std::vector < std::unique_ptr<juce::RangedAudioParameter>>& paramVector);
    void prepareParameter(std::unique_ptr<juce::AudioProcessorValueTreeState>&  vts);
    
    // some necessary info for the host
    int getLatency(){return m_Latency;};

private:
	FormantFilterAudioProcessor* m_processor;
    int m_Latency = 0;

	FormantFilter m_filter;

	jade::AudioProcessParameter<float> m_paramF1Frequency;
	float m_F1Frequency = g_paramF1Frequency.defaultValue;
	jade::AudioProcessParameter<float> m_paramF2Frequency;
	float m_F2Frequency = g_paramF2Frequency.defaultValue;
	jade::AudioProcessParameter<float> m_paramF1Radius;
	float m_F1Radius = g_paramF1Radius.defaultValue;
	jade::AudioProcessParameter<float> m_paramF2Radius;
	float m_F2Radius = g_paramF2Radius.defaultValue;

};

class FormantFilterGUI : public juce::Component
{
public:
	FormantFilterGUI(FormantFilterAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts);

	void paint(juce::Graphics& g) override;
	void resized() override;
private:
	FormantFilterAudioProcessor& m_processor;
    juce::AudioProcessorValueTreeState& m_apvts; 

	XYComponent m_XYComponent;

};
