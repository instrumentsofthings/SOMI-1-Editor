/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EditorView.h"

//==============================================================================

juce::String EditorView::noteToName(int note)
{
    return juce::MidiMessage::getMidiNoteName(note, true, true, MIDI_NOTE_MIDDLE_C);
}

int EditorView::noteNameToInt(juce::String keyName)
{
    // Took from https://forum.juce.com/t/midimessage-keynametonumber/9904 as workaround
    static const char* const noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "", "Db", "", "Eb", "", "", "Gb", "", "Ab", "", "Bb" };

    int keyNumber, octave = 0, numPos = keyName.indexOfAnyOf("01234567890-");

    if (numPos == 0){
        keyNumber = keyName.getIntValue(); //apparently already a number!
    }
    else
    {
        if (numPos > 0)
        {
            octave = keyName.substring(numPos).getIntValue() + MIDI_NOTE_MIDDLE_C - 1;
        }
        else
        {
            octave = 5; //default to octave of middle C if none found
            numPos = keyName.length();
        }

        juce::String name(keyName.substring(0, numPos).trim().toUpperCase());

        keyNumber = juce::StringArray(noteNames, 12).indexOf(name) % 12;

        if (keyNumber < 0) {
            DBG("Phoney keyname: " << keyName);
        }
    }

    return keyNumber + octave * 12;
}

EditorView::EditorView(SomiDataModel& model)
: sensorHeaderComponent(model),
  controlChangeHeaderComponent(model),
  notesHeaderComponent(model),
  pitchBendHeaderComponent(model),
  sensorComponent(model),
  controlChangeComponent(model),
  notesComponent(model),
  pitchBendComponent(model)
{
    addAndMakeVisible(&sensorHeaderComponent);
    addAndMakeVisible(&controlChangeHeaderComponent);
    addAndMakeVisible(&notesHeaderComponent);
    addAndMakeVisible(&pitchBendHeaderComponent);
    addAndMakeVisible(&sensorComponent);
    addAndMakeVisible(&controlChangeComponent);
    addAndMakeVisible(&notesComponent);
    addAndMakeVisible(&pitchBendComponent);

    // Info component is hidden by default
    addChildComponent(&infoComponent);
        
    // Add global event listeners
    sensorHeaderComponent.addActionListener(this);
}

EditorView::~EditorView()
{
    sensorHeaderComponent.removeActionListener(this);
}

void EditorView::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF333333));
}

void EditorView::resized()
{
    auto display = juce::Desktop::getInstance().getDisplays();
    
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Px = juce::Grid::Px;

    auto topRowHeight = display.physicalToLogical(juce::Rectangle<int>(0, 0, 1915, 460)).getHeight();
    auto bottomRowHeight = display.physicalToLogical(juce::Rectangle<int>(0, 0, 1915, 1320-460)).getHeight();
    auto col1Width = display.physicalToLogical(juce::Rectangle<int>(0, 0, 460, 1320)).getWidth();
    auto col2Width = display.physicalToLogical(juce::Rectangle<int>(0, 0, 945-460, 1320)).getWidth();
    auto col3Width = display.physicalToLogical(juce::Rectangle<int>(0, 0, 1430-945, 1320)).getWidth();
    auto col4Width = display.physicalToLogical(juce::Rectangle<int>(0, 0, 1915-1430, 1320)).getWidth();

    grid.templateRows    = { Track(Px(topRowHeight)), Track(Px(bottomRowHeight)) };
    grid.templateColumns = { Track(Px(col1Width)),
                             Track(Px(col2Width)),
                             Track(Px(col3Width)),
                             Track(Px(col4Width)) };

    grid.items = {  juce::GridItem(sensorHeaderComponent),
                    juce::GridItem(controlChangeHeaderComponent),
                    juce::GridItem(notesHeaderComponent),
                    juce::GridItem(pitchBendHeaderComponent),
                    juce::GridItem(sensorComponent),
                    juce::GridItem(controlChangeComponent),
                    juce::GridItem(notesComponent),
                    juce::GridItem(pitchBendComponent) };

    grid.performLayout(getLocalBounds());
    
    infoComponent.setBounds(display.physicalToLogical(juce::Rectangle<int>(22, 22, 610, 1322-(22*2))));
}

void EditorView::addBtnListener(ActionListener* const listener)
{
    sensorComponent.addActionListener(listener);
    pitchBendComponent.addActionListener(listener);
    controlChangeComponent.addActionListener(listener);
}

void EditorView::removeBtnListener(ActionListener* const listener)
{
    sensorComponent.removeActionListener(listener);
    pitchBendComponent.removeActionListener(listener);
    controlChangeComponent.removeActionListener(listener);
}

void EditorView::actionListenerCallback(const juce::String& message)
{
    if (message.compare(GLOBAL_EVT_INFO_BTN_CLICKED) == 0){        
        infoComponent.setVisible(true);
    }
}

void EditorView::setSensorConnected(int sensorId, bool connected)
{
    sensorHeaderComponent.setSensorConnected(sensorId, connected);
}

void EditorView::updateSensorActivity(int sensorId, float value)
{
    sensorHeaderComponent.updateSensorActivity(sensorId, value);
}
