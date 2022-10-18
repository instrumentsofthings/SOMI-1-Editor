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
#include "SomiDataModel.h"

//==============================================================================
/*
*/
class ControlChangeComponent  : public juce::Component,
                                public juce::Button::Listener,
                                public juce::TextEditor::Listener,
                                public juce::ActionListener,
                                public juce::ActionBroadcaster
{
public:
    enum RadioButtonIds {
        activeSelectButtons = 1001,
        inverseSelectButtons,
        ccHighResSelectButtons,
        soloSelectButtons
    };
    
    ControlChangeComponent(SomiDataModel& model);
    ~ControlChangeComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void actionListenerCallback(const juce::String& message) override;
    
    // Getters
    bool isActive() const;
    bool isInverse() const;
    float getScale() const;
    float getSlewLimiterRise() const;
    float getSlewLimiterFall() const;
    float getSlewLimiterShape() const;
    int getMidiChannel() const;
    int getCcController() const;
    bool isHighResolution() const;
    bool isSolo() const;

private:
    
    // Setters
    // Note: View is only updated from data model, thus private
    void setSelectedMovementParameterName(juce::String paramName);
    void setActive(bool active);
    void setInverse(bool inverse);
    void setScale(float scale);
    void setSlewLimiterShape(float shape);
    void setSlewLimiterRise(float rise);
    void setSlewLimiterFall(float fall);
    void setMidiChannel(int channel);
    void setCcController(int controller);
    void setHighResolution(bool highResEnabled);
    void setSolo(bool soloed);
    
    void buttonClicked(juce::Button* button) override;
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorFocusLost(juce::TextEditor& editor) override;
    
    SomiLookAndFeel somiLf;
    SoloBtnLookAndFeel soloBtnLf;
    
    juce::Label titleLabel,
                activeLabel,
                inverseLabel,
                scaleLabel,
                slewLimiterLabel,
                arrowUpLabel,
                arrowDownLabel,
                expLabel,
                midiLabel,
                bitsLabel,
                soloInfoLabelTop,
                soloInfoLabelBottom,
                soloLabel;
    
    juce::TextButton activeOnButton,
                     activeOffButton,
                     inverseOnButton,
                     inverseOffButton,
                     ccHighResOffButton,
                     ccHighResOnButton,
                     soloInactiveButton,
                     soloActiveButton;
    
    juce::TextEditor scaleParam,
                     slewRiseParam,
                     slewFallParam,
                     slewExpParam,
                     midiChParam,
                     midiControllerParam;
    
    SomiDataModel& dataModel;
    
    // State variables
    bool active,
         inverse,
         highRes,
         solo;
    
    float scale,
          slewRise,
          slewFall,
          slewShape;
    
    int midiChannel,
        midiCcController;
                     
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlChangeComponent)
};
