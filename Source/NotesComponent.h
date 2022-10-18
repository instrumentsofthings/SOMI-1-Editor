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
#include "MovementParameterSelectComponent.h"
#include "SomiDataModel.h"

//==============================================================================
/*
*/
class NotesComponent  : public juce::Component,
                        public juce::Button::Listener,
                        public juce::TextEditor::Listener,
                        public juce::ActionListener
{
public:
    enum RadioButtonIds {
        activeSelectButtons = 1001,
        inverseSelectButtons
    };
    
    NotesComponent(SomiDataModel& model);
    ~NotesComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void actionListenerCallback(const juce::String& message) override;
    
    // Getters
    bool isActive() const;
    bool isGateInverse() const;
    float getThreshold() const;
    motion_param_t getParamNotePitch() const;
    int getNoteMin() const;
    int getNoteMax() const;
    int getMidiChannel() const;
    
private:
    
    // Setters
    // Note: View is only updated from data model, thus private
    void setSelectedMovementParameterGateName(juce::String paramName);
    void setActive(bool active);
    void setInverseGate(bool inverse);
    void setThreshold(float threshold);
    void setSelectedMovementParameterNote(motion_param_t param);
    void setNoteMin(int noteMin);
    void setNoteMax(int noteMax);
    void setMidiChannel(int channel);
    
    void buttonClicked(juce::Button* button) override;
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorFocusLost(juce::TextEditor& editor) override;
    
    SomiLookAndFeel somiLf;
    
    juce::Label titleLabel,
                activeLabel,
                inverseLabel,
                thresholdLabel,
                chooseNotePitchLabelTop,
                chooseNotePitchLabelBottom,
                noteRangeLabel,
                midiLabel;

    juce::TextButton activeOnButton,
                     activeOffButton,
                     inverseOnButton,
                     inverseOffButton;
    
    juce::TextEditor thresholdParam,
                     noteMinParam,
                     noteMaxParam,
                     midiChParam;
    
    MovementParameterSelectComponent notePitchParamSelComponent;
    
    SomiDataModel& dataModel;
    
    // State variables
    bool active,
         inverseGate;
    
    float threshold;
    
    int noteMin,
        noteMax,
        midiChannel;
    
    motion_param_t inputParamPitch;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NotesComponent)
};
