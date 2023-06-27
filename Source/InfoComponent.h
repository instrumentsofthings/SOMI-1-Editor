/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SomiLookAndFeel.h"

//==============================================================================
/*
*/
class InfoComponent : public juce::Component,
                      public juce::Button::Listener
{
public:
    InfoComponent();
    ~InfoComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent& event) override
    {
        // Hide info view (i.e. this component) when clicking on it
        setVisible(false);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        setVisible(false);
    }

private:
    SomiLookAndFeel somiLf;
    InfoTextLookAndFeel infoTextLf;
    
    juce::Label versionLabel;
    
    juce::TextEditor infoText;
    
    std::unique_ptr<juce::DrawableButton> closeButton;
    juce::HyperlinkButton updateButton,
                          manualButton,
                          quickstartButton,
                          supportButton;
    
    juce::Label zoomLabel;
    juce::Slider zoomSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoComponent)
};
