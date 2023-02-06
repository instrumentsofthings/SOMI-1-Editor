/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ControlChangeHeaderComponent.h"

//==============================================================================
ControlChangePanelComponent::ControlChangePanelComponent()
{
    setLookAndFeel(&somiLf);
    
    addAndMakeVisible(&titleLabel);
    titleLabel.setText("Control Change", juce::dontSendNotification);
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
    infoLabelBottomRight.setText("CC:", juce::dontSendNotification);
    infoLabelBottomRight.setColour(juce::Label::textColourId, juce::Colours::white);
}

ControlChangePanelComponent::~ControlChangePanelComponent()
{
    setLookAndFeel(nullptr);
}

void ControlChangePanelComponent::paint (juce::Graphics& g)
{
    
}

void ControlChangePanelComponent::resized()
{
    int posLabelsX = 535 - 460;

    titleLabel.setBounds(juce::Rectangle<int>(posLabelsX, 81, 256, 40));
    infoLabelTopLeft.setBounds(juce::Rectangle<int>(posLabelsX, 168, 68, SomiLookAndFeel::fontSizeSmall));
    infoLabelTopRight.setBounds(juce::Rectangle<int>(posLabelsX+73, 168, 100, SomiLookAndFeel::fontSizeSmall));
    infoLabelBottomLeft.setBounds(juce::Rectangle<int>(posLabelsX, 168+SomiLookAndFeel::fontSizeSmall+5, 86, SomiLookAndFeel::fontSizeSmall));
    infoLabelBottomRight.setBounds(juce::Rectangle<int>(posLabelsX+91, 168+SomiLookAndFeel::fontSizeSmall+5, 50, SomiLookAndFeel::fontSizeSmall));
}

ControlChangeHeaderComponent::ControlChangeHeaderComponent(SomiDataModel& model) : movementParamSelComponent(juce::Colour(SomiLookAndFeel::blue)), dataModel(model), selectedParam(TILT_X)
{
    setLookAndFeel(&somiLf);
    
    addAndMakeVisible(&ccPanelComponent);
    addAndMakeVisible(&movementParamSelComponent);
    
    movementParamSelComponent.activityButton.addListener(this);
    movementParamSelComponent.accXButton.addListener(this);
    movementParamSelComponent.accYButton.addListener(this);
    movementParamSelComponent.accZButton.addListener(this);
    movementParamSelComponent.tiltXButton.addListener(this);
    movementParamSelComponent.tiltYButton.addListener(this);
    movementParamSelComponent.tiltZButton.addListener(this);
}

ControlChangeHeaderComponent::~ControlChangeHeaderComponent()
{
    setLookAndFeel(nullptr);
    
    movementParamSelComponent.activityButton.removeListener(this);
    movementParamSelComponent.accXButton.removeListener(this);
    movementParamSelComponent.accYButton.removeListener(this);
    movementParamSelComponent.accZButton.removeListener(this);
    movementParamSelComponent.tiltXButton.removeListener(this);
    movementParamSelComponent.tiltYButton.removeListener(this);
    movementParamSelComponent.tiltZButton.removeListener(this);
}

void ControlChangeHeaderComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF232323));

    g.setColour(juce::Colours::white.withAlpha(0.5f));
    juce::Line<float> line(juce::Point<float>(getWidth(), 0),
                           juce::Point<float>(getWidth(), getHeight()));
    g.drawLine(line, 1.f);
}

void ControlChangeHeaderComponent::resized()
{    
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    
    grid.templateRows    = { Track (Fr (1)), Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)) };
    
    grid.items = {
        juce::GridItem(ccPanelComponent),
        juce::GridItem(movementParamSelComponent)
    };
        
    grid.performLayout(getLocalBounds());
}

motion_param_t ControlChangeHeaderComponent::getSelectedMovementParam() const
{
    return selectedParam;
}

void ControlChangeHeaderComponent::buttonClicked(juce::Button* button)
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
