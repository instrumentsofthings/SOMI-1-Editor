/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include "SomiDefines.h"
#include "Mapping.h"

#include <optional>

// Future: Use juce::ValueTree (offers serialization, value listeners, etc.)

class SomiDataModel : public juce::ActionBroadcaster
{
public:
    SomiDataModel();
    ~SomiDataModel();

    // Update data model based on SysEx message
    void update(const SysExCmdMessage& sysExCmd);
    
    // Update data model based on view component
    void update(const juce::Component& component);
    
    // Is true if mappings were successfully loaded via MIDI SysEx from SOMI-1
    bool isSynced() const;

    // Searches for corresponding mapping and returns sensor ID if found
    int getSensorIdFromMidiMsg(const juce::MidiMessage& message) const;
    
    // Searches for corresponding mapping and returns input parameter if found
    std::optional<motion_param_t> getParamFromMidiMsg(const juce::MidiMessage& message) const;
    
    // Global getters
    int getSelectedSensor() const;
    int getSelectedNoteSlot() const;
    int getSelectedCcSlot() const;
    int getSelectedPbSlot() const;
    int getBatteryLevel(int sensorId) const;
    bool isSensorConnected(int sensorId) const;
    bool isSomiHubAvaiable() const;
    void setSomiHubAvailable(bool found);
    
    // Note config getters
    bool isNoteSlotEnabled(int sensorId, int slot) const;
    int getNoteChannel(int sensorId, int slot) const;
    motion_param_t getNoteParamGate(int sensorId, int slot) const;
    motion_param_t getNoteParamNote(int sensorId, int slot) const;
    int getNoteMin(int sensorId, int slot) const;
    int getNoteMax(int sensorId, int slot) const;
    int getNoteVelocityOn(int sensorId, int slot) const;
    int getNoteVelocityOff(int sensorId, int slot) const;
    float getNoteThreshold(int sensorId, int slot) const;
    bool isNoteGateInversed(int sensorId, int slot) const;
    bool isNotePitchInversed(int sensorId, int slot) const;
    MidiNoteConfig getNoteCfg(int sensorId, int slot) const;
    
    // Control change config getters
    bool isCcSlotEnabled(int sensorId, int slot) const;
    int getCcChannel(int sensorId, int slot) const;
    motion_param_t getCcParam(int sensorId, int slot) const;
    int getCcController(int sensorId, int slot) const;
    bool isCcHighResolution(int sensorId, int slot) const;
    float getCcScale(int sensorId, int slot) const;
    bool isCcInversed(int sensorId, int slot) const;
    float getCcSlewLimiterShape(int sensorId, int slot) const;
    float getCcSlewLimiterRise(int sensorId, int slot) const;
    float getCcSlewLimiterFall(int sensorId, int slot) const;
    float getCcZeroPosition(int sensorId, int slot) const; // Future use
    MidiControlChangeConfig getCcCfg(int sensorId, int slot) const;
    
    // Pitch bend config getters
    bool isPbSlotEnabled(int sensorId, int slot) const;
    int getPbChannel(int sensorId, int slot) const;
    motion_param_t getPbParam(int sensorId, int slot) const;
    float getPbScale(int sensorId, int slot) const;
    bool isPbInversed(int sensorId, int slot) const;
    bool isPbUnipolar(int sensorId, int slot) const; // Future use
    MidiPitchBendConfig getPbCfg(int sensorId, int slot) const;

private:
    
    // Asserts boundaries
    static void assertArgs(int sensorId, int slot, midi_out_func_t outFunc);

    int selectedSensor,
        selectedNoteSlot,
        selectedCcSlot,
        selectedPbSlot;
    
    bool somiHubFound;
    
    // Contains mappings for each sensor
    std::array<output_mapping_t, MAX_NUM_SENSORS> mappings;

    //juce::ValueTree valueTree;
};
