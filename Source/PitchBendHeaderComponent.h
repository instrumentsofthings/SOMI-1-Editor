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
class PitchBendPanelComponent : public juce::Component
{
public:
    PitchBendPanelComponent();
    ~PitchBendPanelComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    SomiLookAndFeel somiLf;
    
    juce::Label titleLabel,
                infoLabelTopLeft,
                infoLabelTopRight,
                infoLabelBottomLeft,
                infoLabelBottomRight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchBendPanelComponent)
};

class PitchBendHeaderComponent : public juce::Component,
                                 public juce::Button::Listener,
                                 public juce::ActionListener
{
public:
    PitchBendHeaderComponent(SomiDataModel& model);
    ~PitchBendHeaderComponent() override;

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
    
    PitchBendPanelComponent pitchBendPanelComponent;
    MovementParameterSelectComponent movementParamSelComponent;
    
    SomiDataModel& dataModel;
    
    motion_param_t selectedParam;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchBendHeaderComponent)
};
