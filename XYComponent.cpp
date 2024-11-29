#include "XYComponent.h"
#include "FormantFilterAudioHandler.h"

XYComponent::XYComponent(juce::RangedAudioParameter *xParam, juce::RangedAudioParameter *yParam)
 
{
    m_xParam = dynamic_cast<juce::AudioParameterFloat*>(xParam);
    m_yParam = dynamic_cast<juce::AudioParameterFloat*>(yParam);
    m_lastxpos = 0;
    m_lastypos = 0;
    m_mouseDown = false;
}

void XYComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::royalblue);

    g.setColour(juce::Colours::white);
    g.drawEllipse(m_lastxpos - 3, m_lastypos - 3, 6, 6, 3);

}

void XYComponent::resized()
{
}

void XYComponent::mouseDown(const juce::MouseEvent &event)
{
}

void XYComponent::mouseDrag(const juce::MouseEvent &event)
{
    m_lastxpos = event.getPosition().getX();
    m_lastypos = event.getPosition().getY();
    if (m_lastxpos < 0)
        m_lastxpos = 0;
    if (m_lastxpos > getWidth())
        m_lastxpos = getWidth();
    if (m_lastypos < 0)
        m_lastypos = 0;
    if (m_lastypos > getHeight())
        m_lastypos = getHeight();

    float x = static_cast<float>(m_lastxpos) / getWidth();
    float y = static_cast<float>(m_lastypos) / getHeight();

    m_xParam->beginChangeGesture();    
    m_xParam->setValueNotifyingHost(x);
    m_xParam->endChangeGesture();   
    m_yParam->beginChangeGesture();
    m_yParam->setValueNotifyingHost(y);
    m_yParam->endChangeGesture();
    repaint();
}

void XYComponent::mouseUp(const juce::MouseEvent &event)
{
    m_lastxpos = event.getPosition().getX();
    m_lastypos = event.getPosition().getY();
    if (m_lastxpos < 0)
        m_lastxpos = 0;
    if (m_lastxpos > getWidth())
        m_lastxpos = getWidth();
    if (m_lastypos < 0)
        m_lastypos = 0;
    if (m_lastypos > getHeight())
        m_lastypos = getHeight();

    float x = static_cast<float>(m_lastxpos) / getWidth();
    float y = static_cast<float>(m_lastypos) / getHeight();

    m_xParam->beginChangeGesture();    
    m_xParam->setValueNotifyingHost(x);
    m_xParam->endChangeGesture();   
    m_yParam->beginChangeGesture();
    m_yParam->setValueNotifyingHost(y);
    m_yParam->endChangeGesture();

    repaint();
}
