#include "FormantFilterAlgo.h"
// This is interesting
// https://dsp.stackexchange.com/questions/26812/estimating-pole-radius-of-formant-filter-from-bandwidth

// https://pmc.ncbi.nlm.nih.gov/articles/PMC6002811/
FormantFilter::FormantFilter(float fs, int nrofchns, int nrofformants)
:m_fs(fs),m_nrOfChns(nrofchns),m_nrOfFormants(nrofformants)
{
    reset();
}

void FormantFilter::setFrequency(int formantnr, float freq_Hz)
{
    if (formantnr < m_nrOfFormants)
    {
        m_frequencies[formantnr] = freq_Hz;
        computeCoeffs();
    }

}

void FormantFilter::setRadius(int formantnr, float ra)
{
    if (formantnr < m_nrOfFormants)
    {
        m_radius[formantnr] = ra;
        computeCoeffs();
    }

}

void FormantFilter::setSamplerate(float fs)
{
    m_fs = fs;
    computeCoeffs();
}

int FormantFilter::processSamples(juce::AudioBuffer<float> &data)
{
    size_t nrOfChns = static_cast < size_t> (data.getNumChannels());
    size_t nrOfSamples = static_cast < size_t> (data.getNumSamples());
    auto dataPtr = data.getArrayOfWritePointers();
    for (size_t cc = 0 ; cc < nrOfChns; cc++)
    {
        for (size_t kk = 0; kk < nrOfSamples; kk++)
        {
            float out = dataPtr[cc][kk];
            for (size_t ll = 0; ll < m_nrOfFormants; ll++)
            {
                out = m_filter[cc][ll].processOneSample(out);
            }
            dataPtr[cc][kk] = out;
        }
    }

    return 0;
}

void FormantFilter::reset()
{
    m_frequencies.clear();
    m_frequencies.resize(m_nrOfFormants);
    m_radius.clear();
    m_radius.resize(m_nrOfFormants);
    m_filter.clear();
    m_filter.resize(m_nrOfChns);
    for (size_t cc = 0; cc < m_nrOfChns; cc++)
    {
        m_filter[cc].clear();
        m_filter[cc].resize(m_nrOfFormants);
        for (size_t kk = 0; kk< m_nrOfFormants; kk++)
        {
            m_filter[cc][kk].reset();
            m_frequencies[kk] = 0.f;
        }
    }
}

void FormantFilter::setFormantbyVocal(juce::String vocal)
{
    // https://www.static.tu.berlin/fileadmin/www/10002019/Forschung/Formantkarten_des_deutschen_Vokalsystems_01.pdf
    if (vocal == "a")
    {
        setFrequency(0,730.f);
        setRadius(0,0.705);
        setFrequency(1,1090.f);
        setRadius(1,0.629);
        
    }
    if (vocal == "e")
    {
        setFrequency(0,660.f);
        setRadius(0,0.79);
        setFrequency(1,2400.f);
        setRadius(1,0.70);
    }

    if (vocal == "i")
    {
        setFrequency(0,400.f);
        setRadius(0,0.87);
        setFrequency(1,3000.f);
        setRadius(1,0.790);
    }
    if (vocal == "o")
    {
        setFrequency(0,500.f);
        setRadius(0,0.79);
        setFrequency(1,800.f);
        setRadius(1,0.70);
    }
    if (vocal == "u")
    {
        setFrequency(0,300.f);
        setRadius(0,0.842);
        setFrequency(1,1000.f);
        setRadius(1,0.79);
    }


    computeCoeffs();
}

void FormantFilter::computeCoeffs()
{
    for (size_t ff = 0 ; ff < m_frequencies.size(); ff++)
    {
        float a1 = -2.f*m_radius[ff]*cosf(2.f*M_PI*m_frequencies[ff]/m_fs);
        float a2 = m_radius[ff]*m_radius[ff];
        for (size_t cc = 0; cc < m_nrOfChns; cc++ )
        {
            m_filter[cc][ff].setCoeffs(a1,a2);
            m_filter[cc][ff].setGainCompensation(1.f- sqrtf(m_radius[ff]));
        }
    }
}
