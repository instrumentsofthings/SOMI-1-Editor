/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SomiDefines.h"

// TODO: Add noexcept specfifier to functions
// TODO: Use data types defined in C++ core instead of uint8_t, etc.

/**
 * Abstract base class representing MIDI SysEx Command
 */
class SysExCmdMessage {
public:
    SysExCmdMessage();
    virtual ~SysExCmdMessage();

    static SysExCmd getSysExCmd(const juce::MidiMessage &sysExMsg);
    
    virtual juce::MidiMessage createSysExMessage(bool request) const = 0;
    virtual juce::String getDescription() const = 0;
    
    // TODO: Add pure virtual function isEqual() to compare configs
    
protected:
    static void intToSysEx(int32_t val, uint8_t midi_buf[5]);
    static int sysExToInt(const uint8_t midi_buf[5]);
    static void floatToSysex(float val, uint8_t midi_buf[5]);
    static float sysexToFloat(const uint8_t midi_buf[5]);
};


/**
 * Class representing cmd for Control Change solo / unsolo
 */
class CCSoloConfig : public SysExCmdMessage {
public:
    CCSoloConfig(uint8_t channel, uint8_t controller);
    virtual ~CCSoloConfig() override; // TODO: Remove virtual?
    
    juce::MidiMessage createSysExMessage(bool request = false) const override;
    juce::String getDescription() const override;
    
    void setSolo(bool state);
    bool isSoloed() const;
    void setChannel(uint8_t channel);
    int getChannel() const;
    void setController(uint8_t controller);
    int getController() const;
    
private:
    bool soloed;
    uint8_t channel;
    uint8_t controller;
};


/**
 * Class representing cmd to persist current configuration on SOMI-1 hub
 */
class PersistConfig : public SysExCmdMessage {
public:
    juce::MidiMessage createSysExMessage(bool request = false) const override;
    juce::String getDescription() const override;
};


/**
 * Class representing cmd to reset mappings of SOMI-1 hub to factory defaults
 */
class ResetMappingsConfig : public SysExCmdMessage {
public:
    juce::MidiMessage createSysExMessage(bool request = false) const override;
    juce::String getDescription() const override;
};


/**
 * Class representing cmd to reset known sensors of SOMI-1 hub
 */
class ResetSensorsConfig : public SysExCmdMessage {
public:
    juce::MidiMessage createSysExMessage(bool request = false) const override;
    juce::String getDescription() const override;
};


/**
 * Class representing cmd to discover SOMI-1 hub
 */
class DiscoverConfig : public SysExCmdMessage {
public:
    juce::MidiMessage createSysExMessage(bool request = true) const override;
    
    juce::String getDescription() const override;
};


/**
 * Class representing cmd to get battery level of sensors
 */
class BatteryLevelConfig : public SysExCmdMessage {
public:
    BatteryLevelConfig(uint8_t sensorID, uint8_t batteryLevel);
    BatteryLevelConfig(const juce::MidiMessage& sysExMsg);
    virtual ~BatteryLevelConfig() override; // TODO: Remove virtual
    
    juce::MidiMessage createSysExMessage(bool request = true) const override;
    juce::String getDescription() const override;
    
    bool isSensorConnected() const;
    void setSensorId(uint8_t sensorID);
    int getSensorId() const;
    void setBatteryLevel(uint8_t batteryLevel);
    int getBatteryLevel() const;
    
private:
    uint8_t sensorID;
    uint8_t batteryLevel;
};


/**
 * Class representing cmd to disable all mappings
 */
class DisableSlotsConfig : public SysExCmdMessage {
public:
    juce::MidiMessage createSysExMessage(bool request = true) const override;
};


/**
 * Abstract base class representing MIDI configuration
 */
class MidiConfig : public SysExCmdMessage {
public:
    MidiConfig(uint8_t sensorID, bool enabled, uint8_t slot, uint8_t channel);
    MidiConfig(const juce::String& jsonString);
    MidiConfig(const juce::MidiMessage& sysExMsg);
    virtual ~MidiConfig();
        
    int getSensorID() const;
    bool isEnabled() const;
    MidiConfig& setEnabled(bool enable);
    int getSlot() const;
    int getChannel() const;
    
protected:
    uint8_t sensorID;
    bool enabled;
    uint8_t slot;
    uint8_t channel;
};


/**
 * Class representing MIDI Note configuration
 */
class MidiNoteConfig : public MidiConfig {
public:    
    MidiNoteConfig(uint8_t sensorID,
                   bool enabled,
                   uint8_t slot,
                   uint8_t channel,
                   motion_param_t inputParamGate,
                   motion_param_t inputParamNote,
                   uint8_t noteMin,
                   uint8_t noteMax,
                   uint8_t velocityNoteOn,
                   uint8_t velocityNoteOff,
                   float threshold,
                   bool inverseGateParam,
                   bool inverseNoteParam);
    MidiNoteConfig(const juce::String& jsonString);
    MidiNoteConfig(const juce::MidiMessage& sysExMsg);
    virtual ~MidiNoteConfig() override; // TODO: Remove override?
    
    juce::MidiMessage createSysExMessage(bool request) const override;
    juce::String getDescription() const override;
    
    motion_param_t getInputParamGate() const;
    motion_param_t getInputParamNote() const;
    int getNoteMin() const;
    int getNoteMax() const;
    int getVelocityOn() const;
    int getVelocityOff() const;
    float getThreshold() const;
    bool isParamGateInversed() const;
    bool isParamNoteInversed() const;
        
private:
    
    motion_param_t inputParamGate;
    motion_param_t inputParamNote;
    uint8_t noteMin;
    uint8_t noteMax;
    uint8_t velocityNoteOn;
    uint8_t velocityNoteOff;
    float threshold;
    bool inverseGateParam;
    bool inverseNoteParam;
};


/**
 * Class representing MIDI Control Change configuration
 */
class MidiControlChangeConfig : public MidiConfig {
public:
    MidiControlChangeConfig(uint8_t sensorID,
                            bool enabled,
                            uint8_t slot,
                            uint8_t channel,
                            motion_param_t inputParam,
                            uint8_t controller,
                            float scale,
                            float slewLimiterShape,
                            float slewLimiterRise,
                            float slewLimiterFall,
                            bool highResolution,
                            bool inverseParam);
    MidiControlChangeConfig(const juce::String& jsonString);
    MidiControlChangeConfig(const juce::MidiMessage& sysExMsg);
    virtual ~MidiControlChangeConfig() override; // TODO: Remove override?
    
    juce::MidiMessage createSysExMessage(bool request) const override;
    juce::String getDescription() const override;
    
    motion_param_t getInputParam() const;
    int getController() const;
    bool isHighResolution() const;
    float getScale() const;
    bool isParamInversed() const;
    float getSlewLimiterShape() const;
    float getSlewLimiterRise() const;
    float getSlewLimiterFall() const;
    //float getZeroPosition() const;
    
    //MidiControlChangeConfig& setHighResolution(bool highResolution);
    
private:
    
    motion_param_t inputParam;
    uint8_t controller;
    bool highResolution;
    float scale;
    bool inverseParam;
    float slewLimiterShape;
    float slewLimiterRise;
    float slewLimiterFall;
    //float zeroPosition;
};


/**
 * Class representing MIDI Pitch Bend configuration
 */
class MidiPitchBendConfig : public MidiConfig {
public:
    MidiPitchBendConfig(uint8_t sensorID,
                     bool enabled,
                     uint8_t slot,
                     uint8_t channel,
                     motion_param_t inputParam,
                     float scale,
                     bool inverseParam);
    MidiPitchBendConfig(const juce::String& jsonString);
    MidiPitchBendConfig(const juce::MidiMessage& sysExMsg);
    virtual ~MidiPitchBendConfig() override; // TODO: Remove override?
    
    juce::MidiMessage createSysExMessage(bool request) const override;
    juce::String getDescription() const override;
    
    motion_param_t getInputParam() const;
    float getScale() const;
    bool isParamInversed() const;
    //bool isUnipolar() const;
    
private:
    
    motion_param_t inputParam;
    float scale;
    bool inverseParam;
    //bool unipolar;
};
