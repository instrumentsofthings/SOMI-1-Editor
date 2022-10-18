/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SensorHeaderComponent.h"
#include "ControlChangeHeaderComponent.h"
#include "NotesHeaderComponent.h"
#include "PitchBendHeaderComponent.h"
#include "SensorComponent.h"
#include "ControlChangeComponent.h"
#include "NotesComponent.h"
#include "PitchBendComponent.h"
#include "InfoComponent.h"

#include "SomiDataModel.h"

//==============================================================================
/*
*/
class EditorView  : public juce::Component,
                    public juce::ActionListener
{
public:
    
    static juce::String noteToName(int note);
    static int noteNameToInt(juce::String noteName);
    
    EditorView(SomiDataModel& model);
    ~EditorView() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void addBtnListener(ActionListener* const listener);
    void removeBtnListener(ActionListener* const listener);
    
    void actionListenerCallback(const juce::String& message) override;

    void setSensorConnected(int sensorId, bool connected);
    void updateSensorActivity(int sensorId, float value);
    
    class SensorActivityCallback : public juce::CallbackMessage
    {
    public:
        SensorActivityCallback(EditorView* o, int sensorId, float alpha)
            : owner(o), sensorId(sensorId), alpha(alpha)
        {}
        
        void messageCallback() override
        {
            if (owner != nullptr)
                owner->updateSensorActivity(sensorId, alpha);
        }
        
        Component::SafePointer<EditorView> owner;
        int sensorId;
        float alpha;
    };

private:
    
    SensorHeaderComponent sensorHeaderComponent;
    ControlChangeHeaderComponent controlChangeHeaderComponent;
    NotesHeaderComponent notesHeaderComponent;
    PitchBendHeaderComponent pitchBendHeaderComponent;
    SensorComponent sensorComponent;
    ControlChangeComponent controlChangeComponent;
    NotesComponent notesComponent;
    PitchBendComponent pitchBendComponent;
    InfoComponent infoComponent;
    
    static juce::Displays::Display scaledDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorView)
};
