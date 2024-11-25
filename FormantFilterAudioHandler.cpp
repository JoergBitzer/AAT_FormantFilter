#include <math.h>
#include "FormantFilterAudioHandler.h"

#include "PluginProcessor.h"

FormantFilterAudio::FormantFilterAudio(FormantFilterAudioProcessor* processor)
:SynchronBlockProcessor(), m_processor(processor)
{
}

void FormantFilterAudio::prepareToPlay(double sampleRate, int max_samplesPerBlock, int max_channels)
{
    juce::ignoreUnused(max_samplesPerBlock,max_channels);
    int synchronblocksize;
    synchronblocksize = static_cast<int>(round(g_desired_blocksize_ms * sampleRate * 0.001)); // 0.001 to transform ms to seconds;
    if (g_forcePowerOf2)
    {
        int nextpowerof2 = int(log2(synchronblocksize))+1;
        synchronblocksize = int(pow(2,nextpowerof2));
    }
    prepareSynchronProcessing(max_channels,synchronblocksize);
    m_Latency += synchronblocksize;
    // here your code
    m_filter.setSamplerate(sampleRate);
    m_filter.setNrOfChns(max_channels);
    m_filter.setnrofFormants(2);
    m_filter.setFormantbyVocal("a");

}

int FormantFilterAudio::processSynchronBlock(juce::AudioBuffer<float> & buffer, juce::MidiBuffer &midiMessages, int NrOfBlocksSinceLastProcessBlock)
{
    juce::ignoreUnused(midiMessages, NrOfBlocksSinceLastProcessBlock);

    bool somethingchanged = false;
    somethingchanged = m_paramF1Frequency.updateWithNotification(m_F1Frequency);
    if (somethingchanged)
    {
        m_filter.setFrequency(0,m_F1Frequency);
    }
    somethingchanged = m_paramF2Frequency.updateWithNotification(m_F2Frequency);
    if (somethingchanged)
    {
        m_filter.setFrequency(1,m_F2Frequency);
    }
    somethingchanged = m_paramF1Radius.updateWithNotification(m_F1Radius);
    if (somethingchanged)
    {
        m_filter.setRadius(0,m_F1Radius);
    }
    somethingchanged = m_paramF2Radius.updateWithNotification(m_F2Radius);
    if (somethingchanged)
    {
        m_filter.setRadius(1,m_F2Radius);
    }

    m_filter.processSamples(buffer);
    return 0;
}

void FormantFilterAudio::addParameter(std::vector<std::unique_ptr<juce::RangedAudioParameter>> &paramVector)
{
    // this is just a placeholder (necessary for compiling/testing the template)
    paramVector.push_back(std::make_unique<AudioParameterFloat>(g_paramF1Frequency.ID,
        g_paramF1Frequency.name,
        NormalisableRange<float>(g_paramF1Frequency.minValue, g_paramF1Frequency.maxValue),
        g_paramF1Frequency.defaultValue,
        AudioParameterFloatAttributes().withLabel (g_paramF1Frequency.unitName)
                                        .withCategory (juce::AudioProcessorParameter::genericParameter)
                                        // or two additional lines with lambdas to convert data for display
                                        .withStringFromValueFunction (std::move ([](float value, int MaxLen) { value = int((value) * 10 + 0.5f) * 0.1f;  return (String(value, MaxLen)); }))
                                        // .withValueFromStringFunction (std::move ([](const String& text) {return text.getFloatValue(); }))
                        ));

    paramVector.push_back(std::make_unique<AudioParameterFloat>(g_paramF2Frequency.ID,
        g_paramF2Frequency.name,
        NormalisableRange<float>(g_paramF2Frequency.minValue, g_paramF2Frequency.maxValue),
        g_paramF2Frequency.defaultValue,
        AudioParameterFloatAttributes().withLabel (g_paramF2Frequency.unitName)
                                        .withCategory (juce::AudioProcessorParameter::genericParameter)
                                        // or two additional lines with lambdas to convert data for display
                                        .withStringFromValueFunction (std::move ([](float value, int MaxLen) { value = int((value) * 10 + 0.5f) * 0.1f;  return (String(value, MaxLen)); }))
                                        // .withValueFromStringFunction (std::move ([](const String& text) {return text.getFloatValue(); }))
                        ));

    paramVector.push_back(std::make_unique<AudioParameterFloat>(g_paramF1Radius.ID,
        g_paramF1Radius.name,
        NormalisableRange<float>(g_paramF1Radius.minValue, g_paramF1Radius.maxValue),
        g_paramF1Radius.defaultValue,
        AudioParameterFloatAttributes().withLabel (g_paramF1Radius.unitName)
                                        .withCategory (juce::AudioProcessorParameter::genericParameter)
                                        // or two additional lines with lambdas to convert data for display
                                        .withStringFromValueFunction (std::move ([](float value, int MaxLen) { value = int((value) * 10 + 0.5f) * 0.1f;  return (String(value, MaxLen)); }))
                                        // .withValueFromStringFunction (std::move ([](const String& text) {return text.getFloatValue(); }))
                        ));

    paramVector.push_back(std::make_unique<AudioParameterFloat>(g_paramF2Radius.ID,
        g_paramF2Radius.name,
        NormalisableRange<float>(g_paramF2Radius.minValue, g_paramF2Radius.maxValue),
        g_paramF2Radius.defaultValue,
        AudioParameterFloatAttributes().withLabel (g_paramF2Radius.unitName)
                                        .withCategory (juce::AudioProcessorParameter::genericParameter)
                                        // or two additional lines with lambdas to convert data for display
                                        .withStringFromValueFunction (std::move ([](float value, int MaxLen) { value = int((value) * 10 + 0.5f) * 0.1f;  return (String(value, MaxLen)); }))
                                        // .withValueFromStringFunction (std::move ([](const String& text) {return text.getFloatValue(); }))
                        ));

}

void FormantFilterAudio::prepareParameter(std::unique_ptr<juce::AudioProcessorValueTreeState> &vts)
{
    m_paramF1Frequency.prepareParameter(vts->getRawParameterValue(g_paramF1Frequency.ID));
    m_paramF2Frequency.prepareParameter(vts->getRawParameterValue(g_paramF2Frequency.ID));
    m_paramF1Radius.prepareParameter(vts->getRawParameterValue(g_paramF1Radius.ID));
    m_paramF2Radius.prepareParameter(vts->getRawParameterValue(g_paramF2Radius.ID));
}


FormantFilterGUI::FormantFilterGUI(FormantFilterAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
:m_processor(p) ,m_apvts(apvts)
{
    
}

void FormantFilterGUI::paint(juce::Graphics &g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId).brighter(0.3f));

    g.setColour (juce::Colours::white);
    g.setFont (12.0f);
    
    juce::String text2display = "FormantFilter V " + juce::String(PLUGIN_VERSION_MAJOR) + "." + juce::String(PLUGIN_VERSION_MINOR) + "." + juce::String(PLUGIN_VERSION_PATCH);
    g.drawFittedText (text2display, getLocalBounds(), juce::Justification::bottomLeft, 1);

}

void FormantFilterGUI::resized()
{
	auto r = getLocalBounds();
    
    // if you have to place several components, use scaleFactor
    //int width = r.getWidth();
	//float scaleFactor = float(width)/g_minGuiSize_x;

    // use the given canvas in r
    juce::ignoreUnused(r);
}
