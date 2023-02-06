/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include "SomiDefines.h"

/**
 * Main Look and Feel class for SOMI-1
 */
class SomiLookAndFeel : public juce::LookAndFeel_V4
{
public:
    enum somiColours : uint32_t {
        yellow = SOMI_COLOR_YELLOW,
        blue = SOMI_COLOR_BLUE
    };
    
    enum fontSizes {
        fontSizeSmall = 23,
        fontSizeBig = 32
    };

    enum offsets {
        offsetUnderline = 7
    };
     
    static const juce::Font& getFontInterLight()
    {
        static juce::Font interLight(juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::InterLight_ttf, BinaryData::InterLight_ttfSize)));
                
        return interLight;
    }
    
    static const juce::Font& getFontInterExtraLight()
    {
        static juce::Font interExtraLight(juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::InterExtraLight_ttf, BinaryData::InterExtraLight_ttfSize)));
                
        return interExtraLight;
    }
    
    SomiLookAndFeel()
    {
        setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.5f));
        setColour(juce::TextButton::textColourOnId, juce::Colours::white);
        setColour(juce::TextButton::textColourOffId, juce::Colours::white.withAlpha(0.25f));
        setColour(juce::TextButton::buttonOnColourId, juce::Colours::white);
        setColour(juce::TextButton::buttonColourId, juce::Colours::white.withAlpha(0.25f));
        setColour(juce::TextEditor::textColourId, juce::Colours::white.withAlpha(0.5f));
        setColour(juce::HyperlinkButton::textColourId, juce::Colours::white);
    }
    
    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        auto labelArea = label.getLocalBounds();
        
        const juce::Font font(getFontInterExtraLight().withHeight(labelArea.toFloat().getHeight()));
        g.setColour (label.findColour(juce::Label::textColourId));
        g.setFont(font);
        g.drawFittedText(label.getText(), labelArea, juce::Justification::centredLeft, 1);
    }
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isButtonHighlighted, bool isButtonDown) override
    {
        // Do nothing
    }
    
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isButtonHighlighted, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        
        // Convert font pixel size to logical
        auto logicalFontHeight = juce::Point<float>(0, fontSizeSmall).getY();
        
        const juce::Font font(getFontInterLight().withHeight(logicalFontHeight));
        g.setFont(font);
        g.setColour(button.findColour(isButtonDown || button.getToggleState() ? juce::TextButton::textColourOnId : juce::TextButton::textColourOffId));
        g.drawText(button.getButtonText(), buttonArea, juce::Justification::centredTop, 1);
        
        auto startUnderline = juce::Point<float>(0, buttonArea.getHeight());
        auto endUnderline = juce::Point<float>(buttonArea.getWidth(), buttonArea.getHeight());
        
        juce::Line<float> line(startUnderline, endUnderline);
        g.drawLine(line, 2.f);
    }
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& editor) override
    {

    }
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& editor) override
    {
        auto editorArea = editor.getLocalBounds();
        
        editor.setIndents(editor.getLeftIndent()/2+1, 0);
        editor.setBorder(juce::BorderSize<int>(0));
        editor.setJustification(juce::Justification::centredTop);

        g.setColour(editor.findColour(juce::TextEditor::textColourId));
        
        auto startUnderline = juce::Point<float>(0, editorArea.getHeight());
        auto endUnderline = juce::Point<float>(editorArea.getWidth(), editorArea.getHeight());
        
        juce::Line<float> line(startUnderline, endUnderline);
        g.drawLine(line, 2.f);
    }
};

class InfoTextLookAndFeel : public juce::LookAndFeel_V4
{
public:
    InfoTextLookAndFeel()
    {

    }
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& editor) override
    {

    }
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& editor) override
    {

    }
};

class RoundTextButtonLookAndFeel : public SomiLookAndFeel
{
public:
    RoundTextButtonLookAndFeel()
    {
        setColour(juce::TextButton::textColourOnId, juce::Colours::white.withAlpha(0.3f));
        setColour(juce::TextButton::textColourOffId, juce::Colours::white.withAlpha(0.3f));
        setColour(juce::TextButton::buttonColourId, juce::Colours::white.withAlpha(0.3f));
        setColour(juce::TextButton::buttonOnColourId, juce::Colour(blue));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isButtonHighlighted, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds().toFloat();
        
        g.setColour(backgroundColour);
        
        // outline
        g.setColour(backgroundColour);
        buttonArea.reduce(1.5f, 1.5f);
        
        if (button.getToggleState())
            g.drawEllipse(buttonArea, 1.5f);
        else
            g.drawEllipse(buttonArea, 0.5f);
    }
    
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isButtonHighlighted, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();

        float logicalFontHeight = 0.f;
        if (button.getComponentID().compare("InfoButton") == 0)
            logicalFontHeight = juce::Point<float>(0, fontSizeSmall-2).getY();
        else 
            logicalFontHeight = juce::Point<float>(0, fontSizeBig).getY();

        const juce::Font font(getFontInterLight().withHeight(logicalFontHeight));
        g.setFont(font);
        g.setColour(button.findColour(isButtonDown || button.getToggleState() ? juce::TextButton::textColourOnId : juce::TextButton::textColourOffId));
        g.drawFittedText(button.getButtonText(), buttonArea, juce::Justification::centred, 1);
        
    }
};

// Used to indicate connection state by drawing big border
class RndButtonConnectedLookAndFeel : public RoundTextButtonLookAndFeel {
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isButtonHighlighted, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds().toFloat();
        
        g.setColour(backgroundColour);
        
        // outline
        g.setColour(backgroundColour);
        buttonArea.reduce(1.5f, 1.5f);
        g.drawEllipse(buttonArea, 1.5f);
    }
};

class ApplySaveBtnLookAndFeel : public SomiLookAndFeel
{
public:
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isButtonHighlighted, bool isButtonDown) override
        {
            auto buttonArea = button.getLocalBounds();
            
            auto logicalFontHeight = juce::Point<float>(0, 40).getY();
            
            const juce::Font font(getFontInterLight().withHeight(logicalFontHeight));
            g.setFont(font);
            g.setColour(button.findColour(isButtonDown || button.getToggleState() ? juce::TextButton::textColourOnId : juce::TextButton::textColourOffId));
            g.drawText(button.getButtonText(), buttonArea, juce::Justification::centredTop, 1);
            
            auto startUnderline = juce::Point<float>(0, buttonArea.getHeight());
            auto endUnderline = juce::Point<float>(buttonArea.getWidth(), buttonArea.getHeight());
            
            juce::Line<float> line(startUnderline, endUnderline);
            g.drawLine(line, 2.f);
        }
};

class SoloBtnLookAndFeel : public SomiLookAndFeel
{
public:
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isButtonHighlighted, bool isButtonDown) override
        {
            auto buttonArea = button.getLocalBounds();
            
            // Convert font pixel size to logical
            auto logicalFontHeight = juce::Point<float>(0, fontSizeBig).getY();
            
            const juce::Font font(getFontInterLight().withHeight(logicalFontHeight));
            g.setFont(font);
            g.setColour(button.findColour(isButtonDown || button.getToggleState() ? juce::TextButton::textColourOnId : juce::TextButton::textColourOffId));
            g.drawText(button.getButtonText(), buttonArea, juce::Justification::centredTop, 1);
            
            auto startUnderline = juce::Point<float>(0, buttonArea.getHeight());
            auto endUnderline = juce::Point<float>(buttonArea.getWidth(), buttonArea.getHeight());
            
            juce::Line<float> line(startUnderline, endUnderline);
            g.drawLine(line, 2.f);
        }
};

class FoundSomiIconLookAndFeel : public SomiLookAndFeel
{
public:
    FoundSomiIconLookAndFeel()
    {
        setColour(juce::ToggleButton::textColourId, juce::Colours::grey);
        setColour(juce::ToggleButton::tickColourId, juce::Colour(yellow));
        setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::grey);
    }
    
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool isButtonHighlighted, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds().toFloat();
        
        if (isButtonDown || button.getToggleState()){
            g.setColour(juce::Colour(yellow));
            g.fillEllipse(buttonArea);
        }
        
        // outline
        buttonArea.reduce(1.f, 1.f);
        if (!isButtonDown && !button.getToggleState()){
            g.setColour(juce::Colours::grey);
            g.drawEllipse(buttonArea, 1.f);
        }
    }
};
