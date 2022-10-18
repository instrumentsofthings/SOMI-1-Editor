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
class ControlChangePanelComponent : public juce::Component
{
public:
    ControlChangePanelComponent();
    ~ControlChangePanelComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    SomiLookAndFeel somiLf;
    
    juce::Label titleLabel,
                infoLabelTopLeft,
                infoLabelTopRight,
                infoLabelBottomLeft,
                infoLabelBottomRight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlChangePanelComponent)
};

class ControlChangeHeaderComponent : public juce::Component,
                                     public juce::Button::Listener
{
public:
    enum RadioButtonIds
    {
        ParameterSelectButtons = 1001
    };
    
    ControlChangeHeaderComponent(SomiDataModel& model);
    ~ControlChangeHeaderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // Getters
    motion_param_t getSelectedMovementParam() const;

private:
    
    // Setters
    void setSelectedMovementParam(motion_param_t param);
    
    void buttonClicked(juce::Button* button) override;
    
    SomiLookAndFeel somiLf;
    
    ControlChangePanelComponent ccPanelComponent;
    MovementParameterSelectComponent movementParamSelComponent;
    
    SomiDataModel& dataModel;
    
    motion_param_t selectedParam;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlChangeHeaderComponent)
};


