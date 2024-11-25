#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI    
    #define M_PI 3.14159265358979323846
#endif
#include <vector>
#include <juce_audio_processors/juce_audio_processors.h>

class SOSAllpol
{
public:
    SOSAllpol(){m_a1 = 0.f; m_a2 = 0.f; m_b0 = 1.f; reset();};
    inline float processOneSample(float x)
    {
        float out = m_b0*x - m_a1*m_state1 - m_a2*m_state2;
        m_state2 = m_state1;
        m_state1 = out;
        return out;
    }
    //virtual int processSamples(juce::AudioBuffer<float>& data);
    void reset(){m_state1 = 0.f; m_state2 = 0.f;};
    void setCoeffs (float a1, float a2){m_a1 = a1; m_a2 = a2;};
    void setGainCompensation(float gain){m_b0 = gain;};

private:
    float m_a1,m_a2,m_state1,m_state2, m_b0;

};

class FormantFilter
{
public:    
    FormantFilter(float fs = 44100.f, int nrofchns = 2, int nrofformants = 2);
    void setFrequency(int formantnr, float freq_Hz);
    void setRadius(int formantnr, float radius);
    void setSamplerate(float fs);
    void setNrOfChns(size_t nrofchns){m_nrOfChns = nrofchns; reset();};
    void setnrofFormants(size_t newnr){m_nrOfFormants = newnr; reset();};
    virtual int processSamples(juce::AudioBuffer<float>& data);
    void reset();
    void setFormantbyVocal (juce::String vocal);


private:
    void computeCoeffs();
    int m_nrOfChns;
    float m_fs;
    float m_nrOfFormants;

    std::vector<float> m_frequencies;
    std::vector<float> m_radius;
    std::vector<std::vector<SOSAllpol> > m_filter;
};