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
class SensorComponent  : public juce::Component,
                         public juce::Button::Listener,
                         public juce::ActionListener,
                         public juce::ActionBroadcaster
{
public:
    SensorComponent(SomiDataModel& model);
    ~SensorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void actionListenerCallback(const juce::String& message) override;
    
    // Setters
    void setBatteryLevel(int batteryLevel);

private:
    
    void buttonClicked(juce::Button* button) override;
    
    SomiLookAndFeel somiLf;
    FoundSomiIconLookAndFeel foundSomiIconLf;
    ApplySaveBtnLookAndFeel applySaveBtnLf;
    
    juce::ToggleButton foundToggleButton;
    
    juce::Label foundSomiLabel,
                batteryLevelLabel;
    
    juce::DrawableImage sensorImage;
    
    juce::TextButton applyButton,
                     saveButton;
    
    SomiDataModel& dataModel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SensorComponent)
};
