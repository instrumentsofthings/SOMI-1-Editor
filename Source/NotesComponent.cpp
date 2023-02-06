/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NotesComponent.h"

#include "Utils.h"
#include "EditorView.h"

//==============================================================================
NotesComponent::NotesComponent(SomiDataModel& model)
: notePitchParamSelComponent(juce::Colours::white),
  dataModel(model),
  active(true),
  inverseGate(false),
  threshold(0.5f),
  noteMin(36),
  noteMax(60),
  midiChannel(0),
  inputParamPitch(TILT_Z)
{
    setLookAndFeel(&somiLf);
    
    addAndMakeVisible(&titleLabel);
    titleLabel.setText("Tilt X:", juce::dontSendNotification);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&activeLabel);
    activeLabel.setText("Active", juce::dontSendNotification);
    
    addAndMakeVisible(&inverseLabel);
    inverseLabel.setText("Inverse", juce::dontSendNotification);
    
    addAndMakeVisible(&thresholdLabel);
    thresholdLabel.setText("Threshold", juce::dontSendNotification);
    
    addAndMakeVisible(&chooseNotePitchLabelTop);
    chooseNotePitchLabelTop.setText("Choose Movement", juce::dontSendNotification);
    chooseNotePitchLabelTop.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&chooseNotePitchLabelBottom);
    chooseNotePitchLabelBottom.setText("Parameter Note Pitch:", juce::dontSendNotification);
    chooseNotePitchLabelBottom.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&noteRangeLabel);
    noteRangeLabel.setText("Note Range", juce::dontSendNotification);
    
    addAndMakeVisible(&midiLabel);
    midiLabel.setText("MIDI", juce::dontSendNotification);
    
    activeOnButton.setButtonText("On");
    activeOnButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&activeOnButton);
    activeOffButton.setButtonText("Off");
    addAndMakeVisible(&activeOffButton);
    
    inverseOnButton.setButtonText("On");
    addAndMakeVisible(&inverseOnButton);
    inverseOffButton.setButtonText("Off");
    inverseOffButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&inverseOffButton);
    
    activeOnButton.setRadioGroupId(activeSelectButtons);
    activeOffButton.setRadioGroupId(activeSelectButtons);
    inverseOnButton.setRadioGroupId(inverseSelectButtons);
    inverseOffButton.setRadioGroupId(inverseSelectButtons);
    
    activeOnButton.setClickingTogglesState(true);
    activeOffButton.setClickingTogglesState(true);
    inverseOnButton.setClickingTogglesState(true);
    inverseOffButton.setClickingTogglesState(true);
    
    addAndMakeVisible(&thresholdParam);
    thresholdParam.setInputRestrictions(4, "0123456789.");
    addAndMakeVisible(&noteMinParam);
    noteMinParam.setInputRestrictions(3, "cdefgabCDEFGAB12345678-#");
    addAndMakeVisible(&noteMaxParam);
    noteMaxParam.setInputRestrictions(3, "cdefgabCDEFGAB12345678-#");
    addAndMakeVisible(&midiChParam);
    midiChParam.setInputRestrictions(2, "0123456789");
    
    addAndMakeVisible(&notePitchParamSelComponent);
    
    activeOnButton.addListener(this);
    activeOffButton.addListener(this);
    inverseOnButton.addListener(this);
    inverseOffButton.addListener(this);
    notePitchParamSelComponent.activityButton.addListener(this);
    notePitchParamSelComponent.accXButton.addListener(this);
    notePitchParamSelComponent.accYButton.addListener(this);
    notePitchParamSelComponent.accZButton.addListener(this);
    notePitchParamSelComponent.tiltXButton.addListener(this);
    notePitchParamSelComponent.tiltYButton.addListener(this);
    notePitchParamSelComponent.tiltZButton.addListener(this);
    
    thresholdParam.addListener(this);
    noteMinParam.addListener(this);
    noteMaxParam.addListener(this);
    midiChParam.addListener(this);
    
    dataModel.addActionListener(this);
}

NotesComponent::~NotesComponent()
{
    setLookAndFeel(nullptr);
    
    activeOnButton.removeListener(this);
    activeOffButton.removeListener(this);
    inverseOnButton.removeListener(this);
    inverseOffButton.removeListener(this);
    notePitchParamSelComponent.activityButton.removeListener(this);
    notePitchParamSelComponent.accXButton.removeListener(this);
    notePitchParamSelComponent.accYButton.removeListener(this);
    notePitchParamSelComponent.accZButton.removeListener(this);
    notePitchParamSelComponent.tiltXButton.removeListener(this);
    notePitchParamSelComponent.tiltYButton.removeListener(this);
    notePitchParamSelComponent.tiltZButton.removeListener(this);
    
    thresholdParam.removeListener(this);
    noteMinParam.removeListener(this);
    noteMaxParam.removeListener(this);
    midiChParam.removeListener(this);
    
    dataModel.removeActionListener(this);
}

void NotesComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF333333));

    g.setColour(juce::Colours::white.withAlpha(0.5f));
    juce::Line<float> lineVertical(juce::Point<float>(getWidth(), 0),
                           juce::Point<float>(getWidth(), getHeight()));
    g.drawLine(lineVertical, 1.f);
    
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    auto lineDim = juce::Rectangle<int>(1008-945, 757-460, 1373-1008, 2);
    juce::Line<float> lineHorizontal1(juce::Point<float>(lineDim.getX(), lineDim.getY()), juce::Point<float>(lineDim.getX()+lineDim.getWidth(), lineDim.getY()));
    g.drawLine(lineHorizontal1);
    
    lineDim = juce::Rectangle<int>(1008-945, 1132-460, 1373-1008, 2);
    juce::Line<float> lineHorizontal2(juce::Point<float>(lineDim.getX(), lineDim.getY()), juce::Point<float>(lineDim.getX()+lineDim.getWidth(), lineDim.getY()));
    g.drawLine(lineHorizontal2);
}

void NotesComponent::resized()
{
    int posLabelsX = 1022 - 945;
    int btnColLeftX = posLabelsX + 136;
    int btnColRightX = posLabelsX + 240;
    int buttonWidth = 66;
    
    titleLabel.setBounds(juce::Rectangle<int>(posLabelsX, 528-460, 256, SomiLookAndFeel::fontSizeSmall));
    activeLabel.setBounds(juce::Rectangle<int>(posLabelsX, 595-460, 120, SomiLookAndFeel::fontSizeSmall));
    inverseLabel.setBounds(juce::Rectangle<int>(posLabelsX, 641-460, 120, SomiLookAndFeel::fontSizeSmall));
    thresholdLabel.setBounds(juce::Rectangle<int>(posLabelsX, 687-460, 120, SomiLookAndFeel::fontSizeSmall));
    chooseNotePitchLabelTop.setBounds(juce::Rectangle<int>(posLabelsX, 800-460, 260, SomiLookAndFeel::fontSizeSmall));
    chooseNotePitchLabelBottom.setBounds(juce::Rectangle<int>(posLabelsX, 800-460+SomiLookAndFeel::fontSizeSmall+5, 260, SomiLookAndFeel::fontSizeSmall));
    noteRangeLabel.setBounds(juce::Rectangle<int>(posLabelsX, 1064-460, 120, SomiLookAndFeel::fontSizeSmall));
    midiLabel.setBounds(juce::Rectangle<int>(posLabelsX, 1205-460, 120, SomiLookAndFeel::fontSizeSmall));
    
    activeOnButton.setBounds(juce::Rectangle<int>(btnColLeftX, 595-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    activeOffButton.setBounds(juce::Rectangle<int>(btnColRightX, 595-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    inverseOnButton.setBounds(juce::Rectangle<int>(btnColLeftX, 641-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    inverseOffButton.setBounds(juce::Rectangle<int>(btnColRightX, 641-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    
    auto logicalFontHeight = juce::Point<float>(0, SomiLookAndFeel::fontSizeSmall).getY();
    const juce::Font font(SomiLookAndFeel::getFontInterExtraLight().withHeight(logicalFontHeight));
    
    thresholdParam.setBounds(juce::Rectangle<int>(btnColLeftX, 687-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    thresholdParam.setFont(font);
    thresholdParam.setText("0.50", juce::dontSendNotification);
    
    noteMinParam.setBounds(juce::Rectangle<int>(btnColLeftX, 1064-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    noteMinParam.setFont(font);
    noteMinParam.setText("C1", juce::dontSendNotification);
    
    noteMaxParam.setBounds(juce::Rectangle<int>(btnColRightX, 1064-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    noteMaxParam.setFont(font);
    noteMaxParam.setText("C3", juce::dontSendNotification);
    
    midiChParam.setBounds(juce::Rectangle<int>(btnColLeftX, 1205-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    midiChParam.setFont(font);
    midiChParam.setText("1", juce::dontSendNotification);
    
    int movementParamBoundOffset = (265-(460/2));
    auto logicalMovementParamBounds = juce::Rectangle<int>(0, 891-460-movementParamBoundOffset, 460, 1047-880);
    notePitchParamSelComponent.setBounds(0, logicalMovementParamBounds.getY(), getWidth(), logicalMovementParamBounds.getHeight());
}

void NotesComponent::actionListenerCallback(const juce::String& message)
{
    if (message.compare(GLOBAL_EVT_DATA_NOTE_CHANGED) == 0){
        
        int sensorId = dataModel.getSelectedSensor();
        int noteSlot = dataModel.getSelectedNoteSlot();
        
        setSelectedMovementParameterGateName(Utils::paramToName(dataModel.getNoteParamGate(sensorId, noteSlot)));
        setActive(dataModel.isNoteSlotEnabled(sensorId, noteSlot));
        setInverseGate(dataModel.isNoteGateInversed(sensorId, noteSlot));
        setThreshold(dataModel.getNoteThreshold(sensorId, noteSlot));
        setSelectedMovementParameterNote(dataModel.getNoteParamNote(sensorId, noteSlot));
        setNoteMin(dataModel.getNoteMin(sensorId, noteSlot));
        setNoteMax(dataModel.getNoteMax(sensorId, noteSlot));
        setMidiChannel(dataModel.getNoteChannel(sensorId, noteSlot));
    }
}

// Getters
bool NotesComponent::isActive() const { return active; }
bool NotesComponent::isGateInverse() const { return inverseGate; }
float NotesComponent::getThreshold() const { return threshold; }
motion_param_t NotesComponent::getParamNotePitch() const { return inputParamPitch; }
int NotesComponent::getNoteMin() const { return noteMin; }
int NotesComponent::getNoteMax() const { return noteMax; }
int NotesComponent::getMidiChannel() const { return midiChannel; }

// Setters
void NotesComponent::setSelectedMovementParameterGateName(juce::String paramName)
{
    titleLabel.setText(paramName + ":", juce::dontSendNotification);
}
void NotesComponent::setActive(bool active)
{
    this->active = active;
    if (active)
        activeOnButton.setToggleState(true, juce::dontSendNotification);
    else
        activeOffButton.setToggleState(true, juce::dontSendNotification);
}
void NotesComponent::setInverseGate(bool inverse)
{
    this->inverseGate = inverse;
    if (inverse)
        inverseOnButton.setToggleState(true, juce::dontSendNotification);
    else
        inverseOffButton.setToggleState(true, juce::dontSendNotification);
}
void NotesComponent::setThreshold(float threshold)
{
    this->threshold = threshold;
    thresholdParam.setText(Utils::ftos(threshold, 2), juce::dontSendNotification);
}
void NotesComponent::setSelectedMovementParameterNote(motion_param_t param)
{
    this->inputParamPitch = param;
    notePitchParamSelComponent.setSelectedMovementParam(param);
}
void NotesComponent::setNoteMin(int noteMin)
{
    this->noteMin = noteMin;
    noteMinParam.setText(EditorView::noteToName(noteMin), juce::dontSendNotification);
}
void NotesComponent::setNoteMax(int noteMax)
{
    this->noteMax = noteMax;
    noteMaxParam.setText(EditorView::noteToName(noteMax), juce::dontSendNotification);
}
void NotesComponent::setMidiChannel(int channel)
{
    this->midiChannel = channel;
    midiChParam.setText(juce::String(midiChannel + 1), juce::dontSendNotification);
}

void NotesComponent::buttonClicked(juce::Button* button)
{
    if (button->getState() && button == &activeOnButton){
        active = true;
    }
    else if (button->getState() && button == &activeOffButton){
        active = false;
    }
    else if (button->getState() && button == &inverseOnButton){
        inverseGate = true;
    }
    else if (button->getState() && button == &inverseOffButton){
        inverseGate = false;
    }
    if (button->getState() && button == &notePitchParamSelComponent.activityButton){
        inputParamPitch = ACTIVITY;
    }
    else if (button->getState() && button == &notePitchParamSelComponent.accXButton){
        inputParamPitch = ACC_X;
    }
    else if (button->getState() && button == &notePitchParamSelComponent.accYButton){
        inputParamPitch = ACC_Y;
    }
    else if (button->getState() && button == &notePitchParamSelComponent.accZButton){
        inputParamPitch = ACC_Z;
    }
    else if (button->getState() && button == &notePitchParamSelComponent.tiltXButton){
        inputParamPitch = TILT_X;
    }
    else if (button->getState() && button == &notePitchParamSelComponent.tiltYButton){
        inputParamPitch = TILT_Y;
    }
    else if (button->getState() && button == &notePitchParamSelComponent.tiltZButton){
        inputParamPitch = TILT_Z;
    }
    
    dataModel.update(*this);
}

void NotesComponent::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    // Just forward event to focus lost listener.
    textEditorFocusLost(editor);
}

void NotesComponent::textEditorFocusLost(juce::TextEditor& editor)
{
    if (&editor == &thresholdParam){
        threshold = std::clamp(editor.getText().getFloatValue(), 0.f, 1.f);
        
        thresholdParam.setText(Utils::ftos(threshold, 2), juce::dontSendNotification);
    }
    else if (&editor == &noteMinParam){
        noteMin = std::clamp(EditorView::noteNameToInt(editor.getText().toUpperCase()), 0, 127);
        
        noteMinParam.setText(EditorView::noteToName(noteMin), juce::dontSendNotification);
    }
    else if (&editor == &noteMaxParam){
        noteMax = std::clamp(EditorView::noteNameToInt(editor.getText().toUpperCase()), 0, 127);
        
        noteMaxParam.setText(EditorView::noteToName(noteMax), juce::dontSendNotification);
    }
    else if (&editor == &midiChParam){
        midiChannel = std::clamp(editor.getText().getIntValue(), 1, 16) - 1;
        
        midiChParam.setText(juce::String(midiChannel + 1), juce::dontSendNotification);
    }
    
    dataModel.update(*this);
}
