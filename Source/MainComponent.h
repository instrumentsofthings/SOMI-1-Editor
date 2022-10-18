/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "EditorView.h"
#include "Mapping.h"
#include "SomiDataModel.h"

// TODO: Dynamically handle MIDI device connect events

//==============================================================================
class MainComponent  : public juce::AudioAppComponent,
                       private juce::MidiInputCallback,
                       private juce::Timer,
                       public juce::ActionListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    
    void actionListenerCallback(const juce::String& message) override;
    
    void enableMidiSomi();
    void requestSettingsFromSomi();
    void sendMidiMessage(const juce::MidiMessage& message);
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void timerCallback() override;
    
    std::unique_ptr<juce::MidiOutput> midiOutput;
    bool somiMidiIoInitialized;
    juce::Array<juce::MidiDeviceInfo> midiInputs;
    juce::Array<juce::MidiDeviceInfo> midiOutputs;

    SomiDataModel dataModel;
    EditorView editorView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
