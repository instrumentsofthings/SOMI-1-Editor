/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PitchBendComponent.h"

#include "Utils.h"

//==============================================================================
PitchBendComponent::PitchBendComponent(SomiDataModel& model)
: dataModel(model),
  active(false),
  inverse(false),
  scale(1.f),
  midiChannel(0)
{
    setLookAndFeel(&somiLf);
    
    addAndMakeVisible(&titleLabel);
    titleLabel.setText("Tilt X:", juce::dontSendNotification);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&activeLabel);
    activeLabel.setText("Active", juce::dontSendNotification);
    
    addAndMakeVisible(&inverseLabel);
    inverseLabel.setText("Inverse", juce::dontSendNotification);
    
    addAndMakeVisible(&scaleLabel);
    scaleLabel.setText("Scale", juce::dontSendNotification);
    
    addAndMakeVisible(&midiLabel);
    midiLabel.setText("MIDI", juce::dontSendNotification);
    
    addAndMakeVisible(&resetLabel);
    resetLabel.setText("Reset:", juce::dontSendNotification);
    
    activeOnButton.setButtonText("On");
    addAndMakeVisible(&activeOnButton);
    activeOffButton.setButtonText("Off");
    activeOffButton.setToggleState(true, juce::dontSendNotification);
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
    
    resetMappingsButton.setButtonText("Mappings");
    addAndMakeVisible(&resetMappingsButton);
    resetSensorsButton.setButtonText("Sensors");
    addAndMakeVisible(&resetSensorsButton);
    
    addAndMakeVisible(&scaleParam);
    scaleParam.setInputRestrictions(4, "0123456789.");
    addAndMakeVisible(&midiChParam);
    midiChParam.setInputRestrictions(2, "0123456789");
    
    activeOnButton.addListener(this);
    activeOffButton.addListener(this);
    inverseOnButton.addListener(this);
    inverseOffButton.addListener(this);
    resetMappingsButton.addListener(this);
    resetSensorsButton.addListener(this);
    
    scaleParam.addListener(this);
    midiChParam.addListener(this);
    
    dataModel.addActionListener(this);
}

PitchBendComponent::~PitchBendComponent()
{
    setLookAndFeel(nullptr);
    
    activeOnButton.removeListener(this);
    activeOffButton.removeListener(this);
    inverseOnButton.removeListener(this);
    inverseOffButton.removeListener(this);
    resetMappingsButton.removeListener(this);
    resetSensorsButton.removeListener(this);
    
    scaleParam.removeListener(this);
    midiChParam.removeListener(this);
    
    dataModel.removeActionListener(this);
}

void PitchBendComponent::paint(juce::Graphics& g)
{
    auto display = juce::Desktop::getInstance().getDisplays();
    
    g.fillAll(juce::Colour(0xFF333333));
    
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    auto lineDim = display.physicalToLogical(juce::Rectangle<int>(1495-1430, 757-460, 1860-1495, 2));
    juce::Line<float> lineHorizontal1(juce::Point<float>(lineDim.getX(), lineDim.getY()), juce::Point<float>(lineDim.getX()+lineDim.getWidth(), lineDim.getY()));
    g.drawLine(lineHorizontal1);
}

void PitchBendComponent::resized()
{
    auto display = juce::Desktop::getInstance().getDisplays();
    
    int posLabelsX = 1509 - 1430;
    int btnColLeftX = posLabelsX + 136;
    int btnColRightX = posLabelsX + 240;
    int buttonWidth = 66;
    
    titleLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 528-460, 256, SomiLookAndFeel::fontSizeSmall)));
    activeLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 595-460, 120, SomiLookAndFeel::fontSizeSmall)));
    inverseLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 641-460, 120, SomiLookAndFeel::fontSizeSmall)));
    scaleLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 687-460, 120, SomiLookAndFeel::fontSizeSmall)));
    midiLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 800-460, 120, SomiLookAndFeel::fontSizeSmall)));
    resetLabel.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX, 1205-460, 140, SomiLookAndFeel::fontSizeSmall)));
    
    activeOnButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColLeftX, 595-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    activeOffButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColRightX, 595-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    inverseOnButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColLeftX, 641-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    inverseOffButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColRightX, 641-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    resetMappingsButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+84, 1205-460, 100, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    resetSensorsButton.setBounds(display.physicalToLogical(juce::Rectangle<int>(posLabelsX+206, 1205-460, 100, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    
    auto logicalFontHeight = display.physicalToLogical(juce::Point<float>(0, SomiLookAndFeel::fontSizeSmall)).getY();
    const juce::Font font(SomiLookAndFeel::getFontInterExtraLight().withHeight(logicalFontHeight));
    
    scaleParam.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColLeftX, 687-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    scaleParam.setFont(font);
    scaleParam.setText("1.00", juce::dontSendNotification);
    
    midiChParam.setBounds(display.physicalToLogical(juce::Rectangle<int>(btnColLeftX, 800-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline)));
    midiChParam.setFont(font);
    midiChParam.setText("1", juce::dontSendNotification);
}

void PitchBendComponent::actionListenerCallback(const juce::String& message)
{
    if (message.compare(GLOBAL_EVT_DATA_PB_CHANGED) == 0){
        
        int sensorId = dataModel.getSelectedSensor();
        int pbSlot = dataModel.getSelectedPbSlot();
        
        setSelectedMovementParameterName(Utils::paramToName(dataModel.getPbParam(sensorId, pbSlot)));
        setActive(dataModel.isPbSlotEnabled(sensorId, pbSlot));
        setInverse(dataModel.isPbInversed(sensorId, pbSlot));
        setScale(dataModel.getPbScale(sensorId, pbSlot));
        setMidiChannel(dataModel.getPbChannel(sensorId, pbSlot));
    }
}

// Getters
bool PitchBendComponent::isActive() const { return active; }
bool PitchBendComponent::isInverse() const { return inverse; }
float PitchBendComponent::getScale() const { return scale; }
int PitchBendComponent::getMidiChannel() const { return midiChannel; }

// Setters
void PitchBendComponent::setSelectedMovementParameterName(juce::String paramName)
{
    titleLabel.setText(paramName + ":", juce::dontSendNotification);
}
void PitchBendComponent::setActive(bool active)
{
    this->active = active;
    if (active)
        activeOnButton.setToggleState(true, juce::dontSendNotification);
    else
        activeOffButton.setToggleState(true, juce::dontSendNotification);
}
void PitchBendComponent::setInverse(bool inverse)
{
    this->inverse = inverse;
    if (inverse)
        inverseOnButton.setToggleState(true, juce::dontSendNotification);
    else
        inverseOffButton.setToggleState(true, juce::dontSendNotification);
}
void PitchBendComponent::setScale(float scale)
{
    this->scale = scale;
    scaleParam.setText(Utils::ftos(scale, 2), juce::dontSendNotification);
}
void PitchBendComponent::setMidiChannel(int channel)
{
    this->midiChannel = channel;
    midiChParam.setText(juce::String(midiChannel + 1), juce::dontSendNotification);
}

void PitchBendComponent::buttonClicked(juce::Button* button)
{
    if (button->getState() && button == &activeOnButton){
        active = true;
    }
    else if (button->getState() && button == &activeOffButton){
        active = false;
    }
    else if (button->getState() && button == &inverseOnButton){
        inverse = true;
    }
    else if (button->getState() && button == &inverseOffButton){
        inverse = false;
    }
    else if (button->getState() && button == &resetMappingsButton){
        sendActionMessage(GLOBAL_EVT_BTN_RST_MAPPINGS);
    }
    else if (button->getState() && button == &resetSensorsButton){
        sendActionMessage(GLOBAL_EVT_BTN_RST_SENSORS);
    }
    
    dataModel.update(*this);
}

void PitchBendComponent::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    // Just forward event to focus lost listener.
    textEditorFocusLost(editor);
}

void PitchBendComponent::textEditorFocusLost(juce::TextEditor& editor)
{
    if (&editor == &scaleParam){
        scale = std::clamp(editor.getText().getFloatValue(), PARAM_RANGE_SCALE_MIN, PARAM_RANGE_SCALE_MAX);
        
        scaleParam.setText(Utils::ftos(scale, 2), juce::dontSendNotification);
    }
    else if (&editor == &midiChParam){
        midiChannel = std::clamp(editor.getText().getIntValue(), 1, 16) - 1;
        
        midiChParam.setText(juce::String(midiChannel + 1), juce::dontSendNotification);
    }
    
    dataModel.update(*this);
}
