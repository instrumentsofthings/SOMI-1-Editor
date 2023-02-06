/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SensorComponent.h"

//==============================================================================
SensorComponent::SensorComponent(SomiDataModel& model)
: dataModel(model)
{
    setLookAndFeel(&somiLf);
    
    // TODO: Replace toggleButton with simple drawable graphic or ignore click events
    foundToggleButton.setLookAndFeel(&foundSomiIconLf);
    addAndMakeVisible(&foundToggleButton);
    foundToggleButton.setClickingTogglesState(false);
    
    addAndMakeVisible(&foundSomiLabel);
    foundSomiLabel.setText("Found SOMI-1", juce::dontSendNotification);
    
    batteryLevelLabel.setText("N.A.", juce::dontSendNotification);
    addAndMakeVisible(&batteryLevelLabel);

    sensorImage.setImage(juce::ImageFileFormat::loadFrom(BinaryData::sensor_figure_png, BinaryData::sensor_figure_pngSize));
    addAndMakeVisible(&sensorImage);
    
    applyButton.setLookAndFeel(&applySaveBtnLf);
    applyButton.setButtonText("Apply");
    applyButton.setColour(juce::TextButton::textColourOnId, juce::Colour(SomiLookAndFeel::yellow).withAlpha(0.25f));
    applyButton.setColour(juce::TextButton::textColourOffId, juce::Colour(SomiLookAndFeel::yellow));
    addAndMakeVisible(&applyButton);
    
    saveButton.setLookAndFeel(&applySaveBtnLf);
    saveButton.setButtonText("Save");
    saveButton.setColour(juce::TextButton::textColourOnId, juce::Colour(SomiLookAndFeel::blue).withAlpha(0.25f));
    saveButton.setColour(juce::TextButton::textColourOffId, juce::Colour(SomiLookAndFeel::blue));
    addAndMakeVisible(&saveButton);
    
    applyButton.addListener(this);
    saveButton.addListener(this);
    
    dataModel.addActionListener(this);
}

SensorComponent::~SensorComponent()
{
    setLookAndFeel(nullptr);
    
    applyButton.removeListener(this);
    saveButton.removeListener(this);
    
    dataModel.removeActionListener(this);
}

void SensorComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF666666));
}

void SensorComponent::resized()
{
    foundToggleButton.setBounds(juce::Rectangle<int>(36, 487-460+5, 24, 24));
    
    foundSomiLabel.setBounds(juce::Rectangle<int>(70, 487-460+6, 130, SomiLookAndFeel::fontSizeSmall));
    batteryLevelLabel.setBounds(juce::Rectangle<int>(369, 487-460, 90, SomiLookAndFeel::fontSizeBig));
    
    sensorImage.setBoundingBox(juce::Rectangle<int>(30, 180, 400, 400).toFloat());
    
    int physicalWidth = juce::Rectangle<int>(0, 0, getWidth(), getHeight()).getWidth();
    int buttonSpace = 96;
    
    applyButton.setBounds(juce::Rectangle<int>(physicalWidth/2 - 100 - buttonSpace/2, 1205-460, 100, 40 + SomiLookAndFeel::offsetUnderline));
    saveButton.setBounds(juce::Rectangle<int>(physicalWidth/2 - 0 + buttonSpace/2, 1205-460, 100, 40 + SomiLookAndFeel::offsetUnderline));
}

void SensorComponent::actionListenerCallback(const juce::String& message)
{
    if (message.compare(GLOBAL_EVT_SENSOR_CHANGED) == 0){
        
        int sensorId = dataModel.getSelectedSensor();
        
        foundToggleButton.setToggleState(dataModel.isSomiHubAvaiable(), juce::dontSendNotification);
        
        if (dataModel.isSensorConnected(sensorId)){
            batteryLevelLabel.setText(juce::String(dataModel.getBatteryLevel(sensorId)) + "%", juce::dontSendNotification);
        }
        else{
            batteryLevelLabel.setText("N.A.", juce::dontSendNotification);
        }
    }
}

void SensorComponent::buttonClicked(juce::Button* button)
{
    if (button->getState() && button == &applyButton){
        sendActionMessage(GLOBAL_EVT_BTN_APPLY_CLICKED);
    }
    else if (button->getState() && button == &saveButton){
        sendActionMessage(GLOBAL_EVT_BTN_SAVE_CLICKED);
    }
}
