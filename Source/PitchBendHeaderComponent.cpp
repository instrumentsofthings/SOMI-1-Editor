/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PitchBendHeaderComponent.h"

//==============================================================================
PitchBendPanelComponent::PitchBendPanelComponent()
{
    setLookAndFeel(&somiLf);
    
    addAndMakeVisible(&titleLabel);
    titleLabel.setText("Pitch Bend", juce::dontSendNotification);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&infoLabelTopLeft);
    infoLabelTopLeft.setText("Choose ", juce::dontSendNotification);
    infoLabelTopLeft.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&infoLabelTopRight);
    infoLabelTopRight.setText("Movement", juce::dontSendNotification);;
    infoLabelTopRight.setColour(juce::Label::textColourId, juce::Colour(SomiLookAndFeel::blue));
    
    addAndMakeVisible(&infoLabelBottomLeft);
    infoLabelBottomLeft.setText("Parameter ", juce::dontSendNotification);
    infoLabelBottomLeft.setColour(juce::Label::textColourId, juce::Colour(SomiLookAndFeel::blue));
    
    addAndMakeVisible(&infoLabelBottomRight);
    infoLabelBottomRight.setText("Pitch Bend:", juce::dontSendNotification);
    infoLabelBottomRight.setColour(juce::Label::textColourId, juce::Colours::white);
}

PitchBendPanelComponent::~PitchBendPanelComponent()
{
    setLookAndFeel(nullptr);
}

void PitchBendPanelComponent::paint (juce::Graphics& g)
{
    
}

void PitchBendPanelComponent::resized()
{
    auto display = juce::Desktop::getInstance().getDisplays();
    
    int posLabelsX = 1509 - 1430;

    titleLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 81, 256, 40)));
    infoLabelTopLeft.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 168, 68, SomiLookAndFeel::fontSizeSmall)));
    infoLabelTopRight.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+73, 168, 100, SomiLookAndFeel::fontSizeSmall)));
    infoLabelBottomLeft.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 168+SomiLookAndFeel::fontSizeSmall+5, 86, SomiLookAndFeel::fontSizeSmall)));
    infoLabelBottomRight.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+91, 168+SomiLookAndFeel::fontSizeSmall+5, 100, SomiLookAndFeel::fontSizeSmall)));
}

PitchBendHeaderComponent::PitchBendHeaderComponent(SomiDataModel& model) : movementParamSelComponent(juce::Colour(SomiLookAndFeel::blue)), dataModel(model), selectedParam(TILT_X)
{
    setLookAndFeel(&somiLf);
    
    addAndMakeVisible(&pitchBendPanelComponent);
    addAndMakeVisible(&movementParamSelComponent);
    
    movementParamSelComponent.activityButton.addListener(this);
    movementParamSelComponent.accXButton.addListener(this);
    movementParamSelComponent.accYButton.addListener(this);
    movementParamSelComponent.accZButton.addListener(this);
    movementParamSelComponent.tiltXButton.addListener(this);
    movementParamSelComponent.tiltYButton.addListener(this);
    movementParamSelComponent.tiltZButton.addListener(this);
    
    dataModel.addActionListener(this);
}

PitchBendHeaderComponent::~PitchBendHeaderComponent()
{
    setLookAndFeel(nullptr);
    
    movementParamSelComponent.activityButton.removeListener(this);
    movementParamSelComponent.accXButton.removeListener(this);
    movementParamSelComponent.accYButton.removeListener(this);
    movementParamSelComponent.accZButton.removeListener(this);
    movementParamSelComponent.tiltXButton.removeListener(this);
    movementParamSelComponent.tiltYButton.removeListener(this);
    movementParamSelComponent.tiltZButton.removeListener(this);
    
    dataModel.removeActionListener(this);
}

void PitchBendHeaderComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF232323));
}

void PitchBendHeaderComponent::resized()
{
    auto display = juce::Desktop::getInstance().getDisplays();
    
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    
    grid.templateRows    = { Track (Fr (1)), Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)) };
    
    grid.items = {
        juce::GridItem(pitchBendPanelComponent),
        juce::GridItem(movementParamSelComponent)
    };
        
    grid.performLayout (getLocalBounds());
}

void PitchBendHeaderComponent::actionListenerCallback(const juce::String& message)
{
    if (message.compare(GLOBAL_EVT_DATA_PB_CHANGED) == 0)
    {
        int sensorId = dataModel.getSelectedSensor();
        int pbSlot = dataModel.getSelectedPbSlot();
        
        setSelectedMovementParam(dataModel.getPbParam(sensorId, pbSlot));
    }
}

// Getters
motion_param_t PitchBendHeaderComponent::getSelectedMovementParam() const
{
    return selectedParam;
}

// Setters
void PitchBendHeaderComponent::setSelectedMovementParam(motion_param_t param)
{
    movementParamSelComponent.setSelectedMovementParam(param);
}

void PitchBendHeaderComponent::buttonClicked(juce::Button* button)
{
    if (button->getState() && button == &movementParamSelComponent.activityButton)
        selectedParam = ACTIVITY;
    else if (button->getState() && button == &movementParamSelComponent.accXButton)
        selectedParam = ACC_X;
    else if (button->getState() && button == &movementParamSelComponent.accYButton)
        selectedParam = ACC_Y;
    else if (button->getState() && button == &movementParamSelComponent.accZButton)
        selectedParam = ACC_Z;
    else if (button->getState() && button == &movementParamSelComponent.tiltXButton)
        selectedParam = TILT_X;
    else if (button->getState() && button == &movementParamSelComponent.tiltYButton)
        selectedParam = TILT_Y;
    else if (button->getState() && button == &movementParamSelComponent.tiltZButton)
        selectedParam = TILT_Z;
    
    dataModel.update(*this);
}
