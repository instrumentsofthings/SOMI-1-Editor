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
class PitchBendComponent  : public juce::Component,
                            public juce::Button::Listener,
                            public juce::TextEditor::Listener,
                            public juce::ActionListener,
                            public juce::ActionBroadcaster
{
public:
    enum RadioButtonIds {
        activeSelectButtons = 1001,
        inverseSelectButtons
    };
    
    PitchBendComponent(SomiDataModel& model);
    ~PitchBendComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void actionListenerCallback(const juce::String& message) override;
    
    // Getters
    bool isActive() const;
    bool isInverse() const;
    float getScale() const;
    int getMidiChannel() const;

private:
    
    // Setters
    // Note: View is only updated from data model, thus private
    void setSelectedMovementParameterName(juce::String paramName);
    void setActive(bool active);
    void setInverse(bool inverse);
    void setScale(float scale);
    void setMidiChannel(int channel);
    
    void buttonClicked(juce::Button* button) override;
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorFocusLost(juce::TextEditor& editor) override;
    
    SomiLookAndFeel somiLf;
    
    juce::Label titleLabel,
                activeLabel,
                inverseLabel,
                scaleLabel,
                midiLabel,
                resetLabel;
    
    juce::TextButton activeOnButton,
                     activeOffButton,
                     inverseOnButton,
                     inverseOffButton,
                     resetMappingsButton,
                     resetSensorsButton;
    
    juce::TextEditor scaleParam,
                     midiChParam;
    
    SomiDataModel& dataModel;
    
    // State variables
    bool active,
         inverse;
    
    float scale;
    
    int midiChannel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchBendComponent)
};
