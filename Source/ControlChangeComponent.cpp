/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ControlChangeComponent.h"

#include "Utils.h"

//==============================================================================
ControlChangeComponent::ControlChangeComponent(SomiDataModel& model)
: dataModel(model),
  active(true),
  inverse(false),
  highRes(false),
  solo(false),
  scale(1.f),
  slewRise(0.f),
  slewFall(0.f),
  slewShape(0.5f),
  midiChannel(0),
  midiCcController(16)
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
    
    addAndMakeVisible(&slewLimiterLabel);
    slewLimiterLabel.setText("Slew Limiter", juce::dontSendNotification);
    
    addAndMakeVisible(&arrowUpLabel);
    arrowUpLabel.setText(juce::CharPointer_UTF8("\xE2\x86\x91"), juce::dontSendNotification);
    
    addAndMakeVisible(&arrowDownLabel);
    arrowDownLabel.setText(juce::CharPointer_UTF8("\xE2\x86\x93"), juce::dontSendNotification);
    
    addAndMakeVisible(&expLabel);
    expLabel.setText("exp.", juce::dontSendNotification);
    
    addAndMakeVisible(&midiLabel);
    midiLabel.setText("MIDI", juce::dontSendNotification);
    
    addAndMakeVisible(&bitsLabel);
    bitsLabel.setText("Bits", juce::dontSendNotification);
    
    addAndMakeVisible(&soloInfoLabelTop);
    soloInfoLabelTop.setText("Solo Movement Parameter for", juce::dontSendNotification);
    soloInfoLabelTop.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&soloInfoLabelBottom);
    soloInfoLabelBottom.setText("MIDI mapping:", juce::dontSendNotification);
    soloInfoLabelBottom.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(&soloLabel);
    soloLabel.setText("Solo:", juce::dontSendNotification);
    
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
    
    ccHighResOffButton.setButtonText("7");
    ccHighResOffButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&ccHighResOffButton);
    ccHighResOnButton.setButtonText("14");
    addAndMakeVisible(&ccHighResOnButton);
    
    soloInactiveButton.setButtonText("Inactive");
    soloInactiveButton.setToggleState(true, juce::dontSendNotification);
    soloInactiveButton.setColour(juce::TextButton::textColourOnId, juce::Colour(SomiLookAndFeel::blue));
    addAndMakeVisible(&soloInactiveButton);
    soloActiveButton.setButtonText("Active");
    soloActiveButton.setColour(juce::TextButton::textColourOnId, juce::Colour(SomiLookAndFeel::blue));
    addAndMakeVisible(&soloActiveButton);
    
    activeOnButton.setRadioGroupId(activeSelectButtons);
    activeOffButton.setRadioGroupId(activeSelectButtons);
    inverseOnButton.setRadioGroupId(inverseSelectButtons);
    inverseOffButton.setRadioGroupId(inverseSelectButtons);
    ccHighResOffButton.setRadioGroupId(ccHighResSelectButtons);
    ccHighResOnButton.setRadioGroupId(ccHighResSelectButtons);
    soloInactiveButton.setRadioGroupId(soloSelectButtons);
    soloActiveButton.setRadioGroupId(soloSelectButtons);
    
    activeOnButton.setClickingTogglesState(true);
    activeOffButton.setClickingTogglesState(true);
    inverseOnButton.setClickingTogglesState(true);
    inverseOffButton.setClickingTogglesState(true);
    ccHighResOffButton.setClickingTogglesState(true);
    ccHighResOnButton.setClickingTogglesState(true);
    soloInactiveButton.setClickingTogglesState(true);
    soloActiveButton.setClickingTogglesState(true);
    
    addAndMakeVisible(&scaleParam);
    scaleParam.setInputRestrictions(4, "0123456789.");
    addAndMakeVisible(&slewRiseParam);
    slewRiseParam.setInputRestrictions(4, "0123456789.");
    addAndMakeVisible(&slewFallParam);
    slewFallParam.setInputRestrictions(4, "0123456789.");
    addAndMakeVisible(&slewExpParam);
    slewExpParam.setInputRestrictions(4, "0123456789%");
    addAndMakeVisible(&midiChParam);
    midiChParam.setInputRestrictions(2, "0123456789");
    addAndMakeVisible(&midiControllerParam);
    midiControllerParam.setInputRestrictions(3, "0123456789");
    
    activeOnButton.addListener(this);
    activeOffButton.addListener(this);
    inverseOnButton.addListener(this);
    inverseOffButton.addListener(this);
    ccHighResOffButton.addListener(this);
    ccHighResOnButton.addListener(this);
    soloInactiveButton.addListener(this);
    soloActiveButton.addListener(this);
    
    scaleParam.addListener(this);
    slewRiseParam.addListener(this);
    slewFallParam.addListener(this);
    slewExpParam.addListener(this);
    midiChParam.addListener(this);
    midiControllerParam.addListener(this);
    
    dataModel.addActionListener(this);
}

ControlChangeComponent::~ControlChangeComponent()
{
    setLookAndFeel(nullptr);
    
    activeOnButton.removeListener(this);
    activeOffButton.removeListener(this);
    inverseOnButton.removeListener(this);
    inverseOffButton.removeListener(this);
    ccHighResOffButton.removeListener(this);
    ccHighResOnButton.removeListener(this);
    soloInactiveButton.removeListener(this);
    soloActiveButton.removeListener(this);
    
    scaleParam.removeListener(this);
    slewRiseParam.removeListener(this);
    slewFallParam.removeListener(this);
    slewExpParam.removeListener(this);
    midiChParam.removeListener(this);
    midiControllerParam.removeListener(this);
    
    dataModel.removeActionListener(this);
}

void ControlChangeComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF333333));

    g.setColour(juce::Colours::white.withAlpha(0.5f));
    juce::Line<float> lineVertical(juce::Point<float>(getWidth(), 0),
                                   juce::Point<float>(getWidth(), getHeight()));
    g.drawLine(lineVertical, 1.f);
    
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    auto lineDim = juce::Rectangle<int>(521-460, 849-460, 888-521, 2);
    juce::Line<float> lineHorizontal1(juce::Point<float>(lineDim.getX(), lineDim.getY()), juce::Point<float>(lineDim.getX()+lineDim.getWidth(), lineDim.getY()));
    g.drawLine(lineHorizontal1);
    
    lineDim = juce::Rectangle<int>(521-460, 1047-460, 888-521, 2);
    juce::Line<float> lineHorizontal2(juce::Point<float>(lineDim.getX(), lineDim.getY()), juce::Point<float>(lineDim.getX()+lineDim.getWidth(), lineDim.getY()));
    g.drawLine(lineHorizontal2);
}

void ControlChangeComponent::resized()
{
    int posLabelsX = 535 - 460;
    int btnColLeftX = posLabelsX + 136;
    int btnColRightX = posLabelsX + 240;
    int buttonWidth = 66;
    
    titleLabel.setBounds(juce::Rectangle<int>(posLabelsX, 528-460, 256, SomiLookAndFeel::fontSizeSmall));
    activeLabel.setBounds(juce::Rectangle<int>(posLabelsX, 595-460, 120, SomiLookAndFeel::fontSizeSmall));
    inverseLabel.setBounds(juce::Rectangle<int>(posLabelsX, 641-460, 120, SomiLookAndFeel::fontSizeSmall));
    scaleLabel.setBounds(juce::Rectangle<int>(posLabelsX, 687-460, 120, SomiLookAndFeel::fontSizeSmall));
    slewLimiterLabel.setBounds(juce::Rectangle<int>(posLabelsX, 733-460, 120, SomiLookAndFeel::fontSizeSmall));
    arrowUpLabel.setBounds(juce::Rectangle<int>(btnColLeftX+buttonWidth+12, 733-460, 20, SomiLookAndFeel::fontSizeSmall));
    arrowDownLabel.setBounds(juce::Rectangle<int>(btnColRightX+buttonWidth+12, 733-460, 20, SomiLookAndFeel::fontSizeSmall));
    expLabel.setBounds(juce::Rectangle<int>(btnColLeftX+buttonWidth+8, 733-460+46, 40, SomiLookAndFeel::fontSizeSmall));
    midiLabel.setBounds(juce::Rectangle<int>(posLabelsX, 891-460, 120, SomiLookAndFeel::fontSizeSmall));
    bitsLabel.setBounds(juce::Rectangle<int>(posLabelsX, 937-460, 120, SomiLookAndFeel::fontSizeSmall));
    soloInfoLabelTop.setBounds(juce::Rectangle<int>(posLabelsX, 1100-460, 260, SomiLookAndFeel::fontSizeSmall));
    soloInfoLabelBottom.setBounds(juce::Rectangle<int>(posLabelsX, 1100-460+SomiLookAndFeel::fontSizeSmall+5, 260, SomiLookAndFeel::fontSizeSmall));
    soloLabel.setBounds(juce::Rectangle<int>(posLabelsX, 1205-460, 80, 36));
    
    activeOnButton.setBounds(juce::Rectangle<int>(btnColLeftX, 595-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    activeOffButton.setBounds(juce::Rectangle<int>(btnColRightX, 595-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    inverseOnButton.setBounds(juce::Rectangle<int>(btnColLeftX, 641-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    inverseOffButton.setBounds(juce::Rectangle<int>(btnColRightX, 641-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    ccHighResOffButton.setBounds(juce::Rectangle<int>(btnColLeftX, 937-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    ccHighResOnButton.setBounds(juce::Rectangle<int>(btnColRightX, 937-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    
    soloInactiveButton.setLookAndFeel(&soloBtnLf);
    soloInactiveButton.setBounds(juce::Rectangle<int>(posLabelsX+100, 1205-460, 110, SomiLookAndFeel::fontSizeBig + SomiLookAndFeel::offsetUnderline));
    soloActiveButton.setLookAndFeel(&soloBtnLf);
    soloActiveButton.setBounds(juce::Rectangle<int>(posLabelsX+240, 1205-460, 110, SomiLookAndFeel::fontSizeBig + SomiLookAndFeel::offsetUnderline));

    auto logicalFontHeight = juce::Point<float>(0, SomiLookAndFeel::fontSizeSmall).getY();
    const juce::Font font(SomiLookAndFeel::getFontInterExtraLight().withHeight(logicalFontHeight));

    scaleParam.setBounds(juce::Rectangle<int>(btnColLeftX, 687-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    scaleParam.setFont(font);
    scaleParam.setText("1.00", juce::dontSendNotification);
    
    slewRiseParam.setBounds(juce::Rectangle<int>(btnColLeftX, 733-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    slewRiseParam.setFont(font);
    slewRiseParam.setText("0.00", juce::dontSendNotification);
    
    slewFallParam.setBounds(juce::Rectangle<int>(btnColRightX, 733-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    slewFallParam.setFont(font);
    slewFallParam.setText("0.00", juce::dontSendNotification);
    
    slewExpParam.setBounds(juce::Rectangle<int>(btnColLeftX, 733-460+46, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    slewExpParam.setFont(font);
    slewExpParam.setText("50%", juce::dontSendNotification);
    
    midiChParam.setBounds(juce::Rectangle<int>(btnColLeftX, 891-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    midiChParam.setFont(font);
    midiChParam.setText("1", juce::dontSendNotification);
    
    midiControllerParam.setBounds(juce::Rectangle<int>(btnColRightX, 891-460, buttonWidth, SomiLookAndFeel::fontSizeSmall + SomiLookAndFeel::offsetUnderline));
    midiControllerParam.setFont(font);
    midiControllerParam.setText("16", juce::dontSendNotification);
}

void ControlChangeComponent::actionListenerCallback(const juce::String& message)
{
    // Update view from data model
    if (message.compare(GLOBAL_EVT_DATA_CC_CHANGED) == 0){
        
        int sensorId = dataModel.getSelectedSensor();
        int ccSlot = dataModel.getSelectedCcSlot();

        setSelectedMovementParameterName(Utils::paramToName(dataModel.getCcParam(sensorId, ccSlot)));
        setActive(dataModel.isCcSlotEnabled(sensorId, ccSlot));
        setInverse(dataModel.isCcInversed(sensorId, ccSlot));
        setScale(dataModel.getCcScale(sensorId, ccSlot));
        setSlewLimiterRise(dataModel.getCcSlewLimiterRise(sensorId, ccSlot));
        setSlewLimiterFall(dataModel.getCcSlewLimiterFall(sensorId, ccSlot));
        setSlewLimiterShape(dataModel.getCcSlewLimiterShape(sensorId, ccSlot));
        setMidiChannel(dataModel.getCcChannel(sensorId, ccSlot));
        setCcController(dataModel.getCcController(sensorId, ccSlot));
        setHighResolution(dataModel.isCcHighResolution(sensorId, ccSlot));
        //setSolo(dataModel.isCcSoloed(sensorId, ccSlot));
    }
}

// Getters
bool ControlChangeComponent::isActive() const { return active; }
bool ControlChangeComponent::isInverse() const { return inverse; }
float ControlChangeComponent::getScale() const { return scale; }
float ControlChangeComponent::getSlewLimiterRise() const { return slewRise; }
float ControlChangeComponent::getSlewLimiterFall() const { return slewFall; }
float ControlChangeComponent::getSlewLimiterShape() const { return slewShape; }
int ControlChangeComponent::getMidiChannel() const { return midiChannel; }
int ControlChangeComponent::getCcController() const { return midiCcController; }
bool ControlChangeComponent::isHighResolution() const { return highRes; }
bool ControlChangeComponent::isSolo() const { return solo; }

// Setters
void ControlChangeComponent::setSelectedMovementParameterName(juce::String paramName)
{
    titleLabel.setText(paramName + ":", juce::dontSendNotification);
}
void ControlChangeComponent::setActive(bool active)
{
    this->active = active;
    if (active)
        activeOnButton.setToggleState(true, juce::dontSendNotification);
    else
        activeOffButton.setToggleState(true, juce::dontSendNotification);
}
void ControlChangeComponent::setInverse(bool inverse)
{
    this->inverse = inverse;
    if (inverse)
        inverseOnButton.setToggleState(true, juce::dontSendNotification);
    else
        inverseOffButton.setToggleState(true, juce::dontSendNotification);
}
void ControlChangeComponent::setScale(float scale)
{
    this->scale = scale;
    scaleParam.setText(Utils::ftos(scale, 2), juce::dontSendNotification);
}
void ControlChangeComponent::setSlewLimiterShape(float shape)
{
    this->slewShape = shape;
    slewExpParam.setText(juce::String(static_cast<int>(slewShape * 100.f)) + "%", juce::dontSendNotification);
}
void ControlChangeComponent::setSlewLimiterRise(float rise)
{
    this->slewRise = rise;
    slewRiseParam.setText(Utils::ftos(slewRise, 2), juce::dontSendNotification);
}
void ControlChangeComponent::setSlewLimiterFall(float fall)
{
    this->slewFall = fall;
    slewFallParam.setText(Utils::ftos(slewFall, 2), juce::dontSendNotification);
}
void ControlChangeComponent::setMidiChannel(int channel)
{
    this->midiChannel = channel;
    midiChParam.setText(juce::String(midiChannel + 1), juce::dontSendNotification);
}
void ControlChangeComponent::setCcController(int controller)
{
    this->midiCcController = controller;
    midiControllerParam.setText(juce::String(midiCcController), juce::dontSendNotification);
}
void ControlChangeComponent::setHighResolution(bool highResEnabled)
{
    this->highRes = highResEnabled;
    if (highResEnabled)
        ccHighResOnButton.setToggleState(true, juce::dontSendNotification);
    else
        ccHighResOffButton.setToggleState(true, juce::dontSendNotification);
}
void ControlChangeComponent::setSolo(bool soloed)
{
    this->solo = soloed;
    if (soloed)
        soloActiveButton.setToggleState(true, juce::dontSendNotification);
    else
        soloInactiveButton.setToggleState(true, juce::dontSendNotification);
}

void ControlChangeComponent::buttonClicked(juce::Button* button)
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
    else if (button->getState() && button == &ccHighResOffButton){
        highRes = false;
    }
    else if (button->getState() && button == &ccHighResOnButton){
        highRes = true;
    }
    else if (button->getState() && button == &soloInactiveButton){
        solo = false;
        sendActionMessage(GLOBAL_EVT_BTN_CC_UNSOLO);
    }
    else if (button->getState() && button == &soloActiveButton){
        solo = true;
        sendActionMessage(GLOBAL_EVT_BTN_CC_SOLO);
    }
    
    dataModel.update(*this);
}

void ControlChangeComponent::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    // Just forward event to focus lost listener.
    textEditorFocusLost(editor);
}

void ControlChangeComponent::textEditorFocusLost(juce::TextEditor& editor)
{
    if (&editor == &scaleParam){
        scale = std::clamp(editor.getText().getFloatValue(), PARAM_RANGE_SCALE_MIN, PARAM_RANGE_SCALE_MAX);
        
        scaleParam.setText(Utils::ftos(scale, 2), juce::dontSendNotification);
    }
    else if (&editor == &slewRiseParam){
        slewRise = std::clamp(editor.getText().getFloatValue(), 0.f, 1.f);
        
        slewRiseParam.setText(Utils::ftos(slewRise, 2), juce::dontSendNotification);
    }
    else if (&editor == &slewFallParam){
        slewFall = std::clamp(editor.getText().getFloatValue(), 0.f, 1.f);
        
        slewFallParam.setText(Utils::ftos(slewFall, 2), juce::dontSendNotification);
    }
    else if (&editor == &slewExpParam){
        slewShape = std::clamp(editor.getText().getFloatValue(), 0.f, 100.f) / 100.f;
        
        slewExpParam.setText(juce::String(static_cast<int>(slewShape * 100.f)) + "%", juce::dontSendNotification);
    }
    else if (&editor == &midiChParam){
        midiChannel = std::clamp(editor.getText().getIntValue(), 1, 16) - 1;
        
        midiChParam.setText(juce::String(midiChannel + 1), juce::dontSendNotification);
    }
    else if (&editor == &midiControllerParam){
        midiCcController = std::clamp(editor.getText().getIntValue(), 0, 127);
        
        midiControllerParam.setText(juce::String(midiCcController), juce::dontSendNotification);
    }
    
    dataModel.update(*this);
}

