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
class NotesPanelComponent : public juce::Component
{
public:
    NotesPanelComponent();
    ~NotesPanelComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    SomiLookAndFeel somiLf;
    
    juce::Label titleLabel,
                infoLabelTopLeft,
                infoLabelTopRight,
                infoLabelBottomLeft,
                infoLabelBottomRight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NotesPanelComponent)
};

class NotesHeaderComponent  : public juce::Component,
                              public juce::Button::Listener,
                              public juce::ActionListener
{
public:
    NotesHeaderComponent(SomiDataModel& model);
    ~NotesHeaderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void actionListenerCallback(const juce::String& message) override;
    
    // Getters
    motion_param_t getSelectedMovementParam() const;

private:
    
    // Setters
    void setSelectedMovementParam(motion_param_t param);
    
    void buttonClicked(juce::Button* button) override;
    
    SomiLookAndFeel somiLf;
    
    NotesPanelComponent notesPanelComponent;
    MovementParameterSelectComponent movementParamSelComponent;
    
    SomiDataModel& dataModel;
    
    motion_param_t selectedParam;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NotesHeaderComponent)
};
