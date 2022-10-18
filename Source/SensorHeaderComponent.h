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
class SensorHeaderComponent  : public juce::Component,
                               public juce::Button::Listener,
                               public juce::ActionBroadcaster,
                               public juce::ActionListener
{
public:
    enum RadioButtonIds
    {
        SensorSelectButtons = 1001
    };
    
    SensorHeaderComponent(SomiDataModel& model);
    ~SensorHeaderComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    // Getters
    int getSelectedSensor() const;
    
    // Setters
    void setSensorConnected(int sensorId, bool connected);
    void updateSensorActivity(int sensorId, float value);
    
private:
    
    void buttonClicked(juce::Button* button) override;
    void actionListenerCallback(const juce::String& message) override;
    
    SomiLookAndFeel somiLf;
    RoundTextButtonLookAndFeel rndTxtBtnLf;
    RndButtonConnectedLookAndFeel rndTxtBtnConnectedLf;
    
    juce::Label titleLabel,
                infoLabelLeft,
                infoLabelRight;
    
    juce::TextButton infoButton,
                     sensorButton1,
                     sensorButton2,
                     sensorButton3,
                     sensorButton4,
                     sensorButton5,
                     sensorButton6;
        
    SomiDataModel& dataModel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SensorHeaderComponent)
};
