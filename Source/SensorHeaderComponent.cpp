/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SensorHeaderComponent.h"
#include "Utils.h"

//==============================================================================
SensorHeaderComponent::SensorHeaderComponent(SomiDataModel& model)
: dataModel(model)
{
    setLookAndFeel(&somiLf);
    
    addAndMakeVisible(&titleLabel);
    titleLabel.setText("Sensor", juce::dontSendNotification);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&infoLabelLeft);
    infoLabelLeft.setText("Choose ", juce::dontSendNotification);
    infoLabelLeft.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&infoLabelRight);
    infoLabelRight.setText("Sensor:", juce::dontSendNotification);
    infoLabelRight.setColour(juce::Label::textColourId, juce::Colour(SomiLookAndFeel::blue));
    
    infoButton.setComponentID("InfoButton");
    infoButton.setLookAndFeel(&rndTxtBtnLf);
    infoButton.setButtonText("?");
    infoButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    infoButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::white);
    infoButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    infoButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(&infoButton);
    
    infoButton.onClick = [this] {
        sendActionMessage(GLOBAL_EVT_INFO_BTN_CLICKED);
    };
    
    sensorButton1.setButtonText("1");
    sensorButton1.setToggleState(true, juce::dontSendNotification);
    sensorButton1.setLookAndFeel(&rndTxtBtnLf);
    addAndMakeVisible(&sensorButton1);
    
    sensorButton2.setButtonText("2");
    sensorButton2.setLookAndFeel(&rndTxtBtnLf);
    addAndMakeVisible(&sensorButton2);
    
    sensorButton3.setButtonText("3");
    sensorButton3.setLookAndFeel(&rndTxtBtnLf);
    addAndMakeVisible(&sensorButton3);
    
    sensorButton4.setButtonText("4");
    sensorButton4.setLookAndFeel(&rndTxtBtnLf);
    addAndMakeVisible(&sensorButton4);
    
    sensorButton5.setButtonText("5");
    sensorButton5.setLookAndFeel(&rndTxtBtnLf);
    addAndMakeVisible(&sensorButton5);
    
    sensorButton6.setButtonText("6");
    sensorButton6.setLookAndFeel(&rndTxtBtnLf);
    addAndMakeVisible(&sensorButton6);
    
    sensorButton1.setRadioGroupId(SensorSelectButtons);
    sensorButton2.setRadioGroupId(SensorSelectButtons);
    sensorButton3.setRadioGroupId(SensorSelectButtons);
    sensorButton4.setRadioGroupId(SensorSelectButtons);
    sensorButton5.setRadioGroupId(SensorSelectButtons);
    sensorButton6.setRadioGroupId(SensorSelectButtons);
    
    sensorButton1.setClickingTogglesState(true);
    sensorButton2.setClickingTogglesState(true);
    sensorButton3.setClickingTogglesState(true);
    sensorButton4.setClickingTogglesState(true);
    sensorButton5.setClickingTogglesState(true);
    sensorButton6.setClickingTogglesState(true);
    
    sensorButton1.addListener(this);
    sensorButton2.addListener(this);
    sensorButton3.addListener(this);
    sensorButton4.addListener(this);
    sensorButton5.addListener(this);
    sensorButton6.addListener(this);
    
    dataModel.addActionListener(this);
}

SensorHeaderComponent::~SensorHeaderComponent()
{
    setLookAndFeel(nullptr);
    
    sensorButton1.removeListener(this);
    sensorButton2.removeListener(this);
    sensorButton3.removeListener(this);
    sensorButton4.removeListener(this);
    sensorButton5.removeListener(this);
    sensorButton6.removeListener(this);
    
    dataModel.removeActionListener(this);
}

void SensorHeaderComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1A1A1A));
}

void SensorHeaderComponent::resized()
{
    auto display = juce::Desktop::getInstance().getDisplays();
    
    int posLabelsX = 88;
    int buttonOffset = 78;
    int posButnsY = 247;

    titleLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 81, 128, 40)));
    infoLabelLeft.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 168, 68, SomiLookAndFeel::fontSizeSmall)));
    infoLabelRight.setBounds(display.physicalToLogical(juce::Rectangle<int>(156+5, 168, 80, SomiLookAndFeel::fontSizeSmall)));
    
    infoButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(396, 30, 40, 40)));
    
    sensorButton1.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, posButnsY, 64, 64)));
    sensorButton2.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+buttonOffset, posButnsY, 64, 64)));
    sensorButton3.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+buttonOffset*2, posButnsY, 64, 64)));
    sensorButton4.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, posButnsY+buttonOffset, 64, 64)));
    sensorButton5.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+buttonOffset, posButnsY+buttonOffset, 64, 64)));
    sensorButton6.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+buttonOffset*2, posButnsY+buttonOffset, 64, 64)));
}

int SensorHeaderComponent::getSelectedSensor() const
{
    if (sensorButton1.getState())
        return 0;
    else if (sensorButton2.getState())
        return 1;
    else if (sensorButton3.getState())
        return 2;
    else if (sensorButton4.getState())
        return 3;
    else if (sensorButton5.getState())
        return 4;
    else if (sensorButton6.getState())
        return 5;
    else
        return 0; // Return default value
}

void SensorHeaderComponent::setSensorConnected(int sensorId, bool connected)
{
    juce::Colour color(connected ? juce::Colours::white : juce::Colours::white.withAlpha(0.3f));
    
    switch(sensorId){
        case 0:
            if (connected)
                sensorButton1.setLookAndFeel(&rndTxtBtnConnectedLf);
            else
                sensorButton1.setLookAndFeel(&rndTxtBtnLf);
            
            sensorButton1.setColour(juce::TextButton::buttonColourId, color);
            break;
        case 1:
            if (connected)
                sensorButton2.setLookAndFeel(&rndTxtBtnConnectedLf);
            else
                sensorButton2.setLookAndFeel(&rndTxtBtnLf);
            
            sensorButton2.setColour(juce::TextButton::buttonColourId, color);
            break;
        case 2:
            if (connected)
                sensorButton3.setLookAndFeel(&rndTxtBtnConnectedLf);
            else
                sensorButton3.setLookAndFeel(&rndTxtBtnLf);
            
            sensorButton3.setColour(juce::TextButton::buttonColourId, color);
            break;
        case 3:
            if (connected)
                sensorButton4.setLookAndFeel(&rndTxtBtnConnectedLf);
            else
                sensorButton4.setLookAndFeel(&rndTxtBtnLf);
            
            sensorButton4.setColour(juce::TextButton::buttonColourId, color);
            break;
        case 4:
            if (connected)
                sensorButton5.setLookAndFeel(&rndTxtBtnConnectedLf);
            else
                sensorButton5.setLookAndFeel(&rndTxtBtnLf);
            
            sensorButton5.setColour(juce::TextButton::buttonColourId, color);
            break;
        case 5:
            if (connected)
                sensorButton6.setLookAndFeel(&rndTxtBtnConnectedLf);
            else
                sensorButton6.setLookAndFeel(&rndTxtBtnLf);
            
            sensorButton6.setColour(juce::TextButton::buttonColourId, color);
            break;
        default:
            std::cerr << "Can't update connection state of sensor " << sensorId << " to view." << std::endl;
            break;
    }
}

void SensorHeaderComponent::updateSensorActivity(int sensorId, float value)
{
    // Remap value to alpha range from 0.25 to 1.0
    float alpha = Utils::map(value, 0.0f, 1.0f, 0.25f, 1.0f, true);
    
    juce::Colour color(juce::Colours::white.withAlpha(alpha));
    
    // Update alpha value for text color of sensor select button
    switch(sensorId){
        case 0:
            sensorButton1.setColour(juce::TextButton::textColourOnId, color);
            sensorButton1.setColour(juce::TextButton::textColourOffId, color);
            break;
        case 1:
            sensorButton2.setColour(juce::TextButton::textColourOnId, color);
            sensorButton2.setColour(juce::TextButton::textColourOffId, color);
            break;
        case 2:
            sensorButton3.setColour(juce::TextButton::textColourOnId, color);
            sensorButton3.setColour(juce::TextButton::textColourOffId, color);
            break;
        case 3:
            sensorButton4.setColour(juce::TextButton::textColourOnId, color);
            sensorButton4.setColour(juce::TextButton::textColourOffId, color);
            break;
        case 4:
            sensorButton5.setColour(juce::TextButton::textColourOnId, color);
            sensorButton5.setColour(juce::TextButton::textColourOffId, color);
            break;
        case 5:
            sensorButton6.setColour(juce::TextButton::textColourOnId, color);
            sensorButton6.setColour(juce::TextButton::textColourOffId, color);
            break;
        default:
            std::cerr << "Can't apply activity of sensor " << sensorId << " to view." << std::endl;
            break;
    }
}

void SensorHeaderComponent::buttonClicked(juce::Button* button)
{
    dataModel.update(*this);
}

void SensorHeaderComponent::actionListenerCallback(const juce::String& message)
{
    if (message.compare(GLOBAL_EVT_SENSOR_CHANGED) == 0){
        
        // Update connection state of all sensors
        for (int sensorId=0; sensorId < MAX_NUM_SENSORS; sensorId++)            setSensorConnected(sensorId, dataModel.isSensorConnected(sensorId));
    }
}
