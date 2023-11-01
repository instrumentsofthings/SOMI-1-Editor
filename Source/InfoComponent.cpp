/*
  ==============================================================================

    InfoComponent.cpp
    Created: 5 Apr 2023 10:26:23am
    Author:  Henrik

  ==============================================================================
*/

#include "InfoComponent.h"

#include "MainComponent.h"

InfoComponent::InfoComponent() :
updateButton("Update", juce::URL(SOMI_URL_UPDATE)),
manualButton("Manual", juce::URL(SOMI_URL_MANUAL)),
quickstartButton("Quick Start Guide", juce::URL(SOMI_URL_QUICK_START)),
supportButton("Support", juce::URL(SOMI_URL_SUPPORT)),
zoomSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    setLookAndFeel(&somiLf);

    closeButton = std::make_unique<juce::DrawableButton>("CloseButton",  juce::DrawableButton::ButtonStyle::ImageFitted);
    closeButton->setImages(juce::Drawable::createFromImageData(BinaryData::icon_button_close_svg, BinaryData::icon_button_close_svgSize).get());
    closeButton->addListener(this);
    addAndMakeVisible(*closeButton);

    addAndMakeVisible(&versionLabel);
    versionLabel.setText(juce::String("Version: ") + ProjectInfo::versionString, juce::dontSendNotification);
    int versionFontHeight = juce::Point<int>(0, SomiLookAndFeel::fontSizeBig).getY();
    juce::Font versionFont(SomiLookAndFeel::getFontInterExtraLight().withHeight(versionFontHeight));
    versionLabel.setFont(versionFont);
    versionLabel.setJustificationType(juce::Justification::left);
    versionLabel.setColour(juce::Label::backgroundColourId, juce::Colours::white.withAlpha(0.5f));
    
    infoText.setLookAndFeel(&infoTextLf);
    infoText.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white.withAlpha(0.5f));
    infoText.setColour(juce::TextEditor::textColourId, juce::Colours::white.withAlpha(0.5f));
    infoText.setColour(juce::TextEditor::highlightedTextColourId, juce::Colours::white.withAlpha(0.5f));
    infoText.setColour(juce::TextEditor::highlightColourId, juce::Colours::white.withAlpha(0.f));
    
    // TODO Don't change mouse cursor when hovering over text
    infoText.setJustification(juce::Justification::topLeft);
    infoText.setMultiLine(true);
    infoText.setReadOnly(true);
    infoText.setBorder(juce::BorderSize<int>(0));
    int infoFontHeight = juce::Point<int>(0, SomiLookAndFeel::fontSizeSmall).getY();
    juce::Font infoFont(SomiLookAndFeel::getFontInterExtraLight().withHeight(infoFontHeight));
    infoText.setFont(infoFont);
    infoText.setText("v" + juce::String(ProjectInfo::versionString) + "\n" + juce::String(juce::CharPointer_UTF8(SOMI_EDITOR_INFO_TEXT)), juce::dontSendNotification);
    addAndMakeVisible(&infoText);
    
    addAndMakeVisible(&updateButton);
    addAndMakeVisible(&manualButton);
    addAndMakeVisible(&quickstartButton);
    addAndMakeVisible(&supportButton);
    
    addAndMakeVisible(&zoomSlider);
    zoomSlider.setRange(50.0, 150.0);
    zoomSlider.setTextValueSuffix(" %");
    zoomSlider.setValue(100.0);
    zoomSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    
    addAndMakeVisible(&zoomLabel);
    zoomLabel.setFont(versionFont);
    zoomLabel.setText("Zoom", juce::dontSendNotification);
    zoomLabel.attachToComponent(&zoomSlider, true);
    zoomLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    zoomLabel.setBorderSize(juce::BorderSize<int>(0));
    
    
    zoomSlider.onValueChange = [this]{
        
        auto physicalDim = juce::Rectangle<float>(0, 0, 1915, 1320); // Took from design template
        
        const float scaleFactor = (1280.0 / physicalDim.getWidth()) * zoomSlider.getValue() / 100.0;
        juce::Desktop::getInstance().setGlobalScaleFactor(scaleFactor);
        
        auto mainComponent = findParentComponentOfClass<MainComponent>();
        jassert(mainComponent != nullptr);
        
        mainComponent->setSize(physicalDim.getWidth(), physicalDim.getHeight());
    };
}

InfoComponent::~InfoComponent()
{
    setLookAndFeel(nullptr);
    
    closeButton->removeListener(this);
}

void InfoComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void InfoComponent::resized()
{
    int offset = 22;
    int xStart = 70 - offset;
    
    closeButton->setBounds(juce::Rectangle<int>(609-64, 49-offset, 48, 48));
    
    versionLabel.setBounds(juce::Rectangle<int>(xStart, 49-offset, 300, SomiLookAndFeel::fontSizeBig));
    
    infoText.setBounds(juce::Rectangle<int>(xStart, 983-offset, 609-70, 14*19));
    
    auto logicalFontHeight = juce::Point<float>(0, SomiLookAndFeel::fontSizeBig).getY();
    juce::Font font(SomiLookAndFeel::getFontInterExtraLight().withHeight(logicalFontHeight));
    
    updateButton.setBounds(juce::Rectangle<int>(xStart, 157-offset, 600, SomiLookAndFeel::fontSizeBig));
    font.setUnderline(true);
    updateButton.setFont(font, false, juce::Justification::topLeft);
    updateButton.changeWidthToFitText();

    manualButton.setBounds(juce::Rectangle<int>(xStart, 248-offset, 600, SomiLookAndFeel::fontSizeBig));
    manualButton.setFont(font, false, juce::Justification::topLeft);
    manualButton.changeWidthToFitText();
    
    quickstartButton.setBounds(juce::Rectangle<int>(xStart, 339-offset, 600, SomiLookAndFeel::fontSizeBig));
    quickstartButton.setFont(font, false, juce::Justification::topLeft);
    quickstartButton.changeWidthToFitText();
    
    supportButton.setBounds(juce::Rectangle<int>(xStart, 430-offset, 600, SomiLookAndFeel::fontSizeBig));
    supportButton.setFont(font, false, juce::Justification::topLeft);
    supportButton.changeWidthToFitText();
    
    int versionFontHeight = juce::Point<int>(0, SomiLookAndFeel::fontSizeBig).getY();
    juce::Font versionFont(SomiLookAndFeel::getFontInterExtraLight().withHeight(versionFontHeight));
    
    int zoomLabelWidth = versionFont.getStringWidth(zoomLabel.getText());
    
    zoomSlider.setBounds(xStart+zoomLabelWidth, supportButton.getY() + supportButton.getHeight() + 60, getWidth()-2*xStart-zoomLabelWidth, SomiLookAndFeel::fontSizeBig);
}
