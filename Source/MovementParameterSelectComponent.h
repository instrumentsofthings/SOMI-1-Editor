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
#include "SomiDefines.h"

//==============================================================================
/*
*/
class MovementParameterSelectComponent : public juce::Component
{
public:
    enum RadioButtonIds
    {
        ParameterSelectButtons = 1001
    };
    
    MovementParameterSelectComponent(juce::Colour buttonColour)
    {
        setLookAndFeel(&somiLf);
        
        addAndMakeVisible(&activityLabel);
        activityLabel.setText("Activity", juce::dontSendNotification);
        activityLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.5f));
        
        addAndMakeVisible(&accelerationLabel);
        accelerationLabel.setText("Acceleration", juce::dontSendNotification);
        accelerationLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.5f));
        
        addAndMakeVisible(&tiltLabel);
        tiltLabel.setText("Tilt Angle", juce::dontSendNotification);
        tiltLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.5f));
        
        activityButton.setButtonText("XYZ");
        activityButton.setColour(juce::TextButton::textColourOnId, buttonColour);
        addAndMakeVisible(&activityButton);
        
        accXButton.setButtonText("X");
        accXButton.setColour(juce::TextButton::textColourOnId, buttonColour);
        addAndMakeVisible(&accXButton);
        
        accYButton.setButtonText("Y");
        accYButton.setColour(juce::TextButton::textColourOnId, buttonColour);
        addAndMakeVisible(&accYButton);
        
        accZButton.setButtonText("Z");
        accZButton.setColour(juce::TextButton::textColourOnId, buttonColour);
        addAndMakeVisible(&accZButton);
        
        tiltXButton.setButtonText("X");
        tiltXButton.setToggleState(true, juce::dontSendNotification);
        tiltXButton.setColour(juce::TextButton::textColourOnId, buttonColour);
        addAndMakeVisible(&tiltXButton);
        
        tiltYButton.setButtonText("Y");
        tiltYButton.setColour(juce::TextButton::textColourOnId, buttonColour);
        addAndMakeVisible(&tiltYButton);
        
        tiltZButton.setButtonText("Z");
        tiltZButton.setColour(juce::TextButton::textColourOnId, buttonColour);
        addAndMakeVisible(&tiltZButton);
        
        activityButton.setRadioGroupId(ParameterSelectButtons);
        accXButton.setRadioGroupId(ParameterSelectButtons);
        accYButton.setRadioGroupId(ParameterSelectButtons);
        accZButton.setRadioGroupId(ParameterSelectButtons);
        tiltXButton.setRadioGroupId(ParameterSelectButtons);
        tiltYButton.setRadioGroupId(ParameterSelectButtons);
        tiltZButton.setRadioGroupId(ParameterSelectButtons);
        
        activityButton.setClickingTogglesState(true);
        accXButton.setClickingTogglesState(true);
        accYButton.setClickingTogglesState(true);
        accZButton.setClickingTogglesState(true);
        tiltXButton.setClickingTogglesState(true);
        tiltYButton.setClickingTogglesState(true);
        tiltZButton.setClickingTogglesState(true);
    }

    ~MovementParameterSelectComponent() override
    {
        setLookAndFeel(nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        juce::Line<float> line(juce::Point<float>(getWidth(), 0),
                               juce::Point<float>(getWidth(), getHeight()));
    }

    void resized() override
    {
        auto display = juce::Desktop::getInstance().getDisplays();
        
        int posLabelsX = 535 - 460;
        int btnColX = posLabelsX + 150;
        int btnColY = posLabelsX + 220;
        int btnColZ = posLabelsX + 290;
        int buttonWidth = 40;
        
        activityLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 265-(460/2), 120, SomiLookAndFeel::fontSizeSmall)));
        accelerationLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 311-(460/2), 120, SomiLookAndFeel::fontSizeSmall)));
        tiltLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 357-(460/2), 120, SomiLookAndFeel::fontSizeSmall)));
        
        activityButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColX, 265-(460/2), buttonWidth, SomiLookAndFeel::fontSizeSmall)));
        accXButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColX, 311-(460/2), buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
        accYButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColY, 311-(460/2), buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
        accZButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColZ, 311-(460/2), buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
        tiltXButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColX, 357-(460/2), buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
        tiltYButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColY, 357-(460/2), buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
        tiltZButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColZ, 357-(460/2), buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    }

    void setSelectedMovementParam(motion_param_t param)
    {
        switch(param){
            case TILT_X:
                tiltXButton.setToggleState(true, juce::dontSendNotification);
                break;
            case TILT_Y:
                tiltYButton.setToggleState(true, juce::dontSendNotification);
                break;
            case TILT_Z:
                tiltZButton.setToggleState(true, juce::dontSendNotification);
                break;
            case ACC_X:
                accXButton.setToggleState(true, juce::dontSendNotification);
                break;
            case ACC_Y:
                accYButton.setToggleState(true, juce::dontSendNotification);
                break;
            case ACC_Z:
                accZButton.setToggleState(true, juce::dontSendNotification);
                break;
            case ACTIVITY:
                activityButton.setToggleState(true, juce::dontSendNotification);
                break;
        }
    }

private:
    
    // Member variables are public as MovementParameterSelectComponent
    // is intended to be embedded in other components only.
    SomiLookAndFeel somiLf;
    
    juce::Label activityLabel,
                accelerationLabel,
                tiltLabel;
    
    juce::TextButton activityButton,
                     accXButton,
                     accYButton,
                     accZButton,
                     tiltXButton,
                     tiltYButton,
                     tiltZButton;

    // Make containing components as friends
    friend class ControlChangeHeaderComponent;
    friend class NotesHeaderComponent;
    friend class PitchBendHeaderComponent;
    friend class NotesComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MovementParameterSelectComponent)
};
