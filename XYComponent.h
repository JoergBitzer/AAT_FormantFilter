#pragma once


#include <juce_audio_processors/juce_audio_processors.h>

class XYComponent : public juce::Component
{
public:
    XYComponent(juce::RangedAudioParameter *xParam, juce::RangedAudioParameter *yParam);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    
private:
    int m_lastxpos;
    int m_lastypos;
    bool m_mouseDown;
    //juce::RangedAudioParameter *m_xParam;
    //juce::RangedAudioParameter *m_yParam;
    juce::AudioParameterFloat *m_xParam;
    juce::AudioParameterFloat *m_yParam;

};



