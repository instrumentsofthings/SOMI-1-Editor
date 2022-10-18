/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include "SomiDataModel.h"

#include "EditorView.h"

//==============================================================================
SomiDataModel::SomiDataModel()
: selectedSensor(0),
  selectedNoteSlot(0),
  selectedCcSlot(0),
  selectedPbSlot(0),
  somiHubFound(false)
/* : valueTree(juce::Identifier("SomiDataModel"))*/
{
    // Initialize sensor data with factory defaults
    for (int i=0; i < MAX_NUM_SENSORS; i++){
        mappings[i].sensor.ID = i;
        mappings[i].sensor.battery_level = 0x7F;
    }
    
    // Initialize mappings
    for (uint8_t i=0; i < MAX_NUM_SENSORS; i++){
        for (uint8_t j=0; j < MAX_NUM_NOTE_MSG_PER_SENSOR; j++){
            mappings[i].note_probs[j].channel = i;
            mappings[i].note_probs[j].input_param_note = TILT_Z;
            mappings[i].note_probs[j].inverse_gate_param = false;
            mappings[i].note_probs[j].inverse_note_param = false;
            mappings[i].note_probs[j].note_max = 60;
            mappings[i].note_probs[j].note_min = 36;
            mappings[i].note_probs[j].velocity_note_off = 0;
            mappings[i].note_probs[j].velocity_note_on = 127;
            mappings[i].note_probs[j].threshold = 0.5;

            if (j == 0)
                mappings[i].note_probs[j].enabled = true;
            else
                mappings[i].note_probs[j].enabled = false;

            if (i % 2 == 1){
                mappings[i].note_probs[j].input_param_gate = TILT_X;
            }
            else{
                mappings[i].note_probs[j].input_param_gate = ACTIVITY;
            }
        }
        for (uint8_t j=0; j < MAX_NUM_CC_MSG_PER_SENSOR; j++){
            mappings[i].cc_probs[j].channel = i;
            mappings[i].cc_probs[j].controller = j + 16; // Starting with general purpose controllers
            if (j == 7)
                mappings[i].cc_probs[j].enabled = false;
            else
                mappings[i].cc_probs[j].enabled = true;
            mappings[i].cc_probs[j].high_resolution = false;
            mappings[i].cc_probs[j].input_param = static_cast<motion_param_t>(j % NUM_MOTION_PARAMS);
            mappings[i].cc_probs[j].inverse = false;
            mappings[i].cc_probs[j].scale = 1.0;
            mappings[i].cc_probs[j].slew_limiter.rise = 0.f;
            mappings[i].cc_probs[j].slew_limiter.fall = 0.f;
            mappings[i].cc_probs[j].slew_limiter.shape = 0.5f;
            mappings[i].cc_probs[j].slew_limiter.sample_time = 1.f / 1600.f;
            mappings[i].cc_probs[j].zero_position = 0.f;
        }
        for (uint8_t j=0; j < MAX_NUM_PB_MSG_PER_SENSOR; j++){
            mappings[i].pb_probs[j].channel = j + i*MAX_NUM_PB_MSG_PER_SENSOR;
            mappings[i].pb_probs[j].enabled = false;
            mappings[i].pb_probs[j].input_param = static_cast<motion_param_t>(ACC_X + j);
            mappings[i].pb_probs[j].inverse = false;
            mappings[i].pb_probs[j].scale = 1.0f;
            mappings[i].pb_probs[j].unipolar = false;
        }
    }
}

SomiDataModel::~SomiDataModel()
{
    
}

void SomiDataModel::update(const SysExCmdMessage& sysExCmd)
{
    if (auto* noteCfg = dynamic_cast<const MidiNoteConfig*>(&sysExCmd))
    {
        try {
            
            // Check boundaries of slots as underlying data structure
            // is based on C array, i.e. no exception is thrown when accessing
            // non available slot.
            if (noteCfg->getSlot() >= MAX_NUM_NOTE_MSG_PER_SENSOR){
                throw std::out_of_range("Can't access note slot " + std::to_string(noteCfg->getSlot()));
            }
            
            midi_note_t& noteDataSlot = mappings.at(noteCfg->getSensorID()).note_probs[noteCfg->getSlot()];
            
            noteDataSlot.enabled = noteCfg->isEnabled();
            noteDataSlot.channel = noteCfg->getChannel();
            noteDataSlot.input_param_gate = noteCfg->getInputParamGate();
            noteDataSlot.input_param_note = noteCfg->getInputParamNote();
            noteDataSlot.note_min = noteCfg->getNoteMin();
            noteDataSlot.note_max = noteCfg->getNoteMax();
            noteDataSlot.velocity_note_on = noteCfg->getVelocityOn();
            noteDataSlot.velocity_note_off = noteCfg->getVelocityOff();
            noteDataSlot.threshold = noteCfg->getThreshold();
            noteDataSlot.inverse_gate_param = noteCfg->isParamGateInversed();
            noteDataSlot.inverse_note_param = noteCfg->isParamNoteInversed();
        }
        catch (const std::exception& e){
            std::cerr << "Exception occured when updating data model with note mapping: " << e.what() << std::endl;
        }
        
        sendActionMessage(GLOBAL_EVT_DATA_NOTE_CHANGED);
    }
    else if (auto* ccCfg = dynamic_cast<const MidiControlChangeConfig*>(&sysExCmd))
    {
        try {
            
            // Check boundaries of slots as underlying data structure
            // is based on C array, i.e. no exception is thrown when accessing
            // non available slot.
            if (ccCfg->getSlot() >= MAX_NUM_CC_MSG_PER_SENSOR){
                throw std::out_of_range("Can't access control change slot " + std::to_string(ccCfg->getSlot()));
            }
            
            midi_cc_t& ccDataSlot = mappings.at(ccCfg->getSensorID()).cc_probs[ccCfg->getSlot()];
            
            ccDataSlot.enabled = ccCfg->isEnabled();
            ccDataSlot.channel = ccCfg->getChannel();
            ccDataSlot.input_param = ccCfg->getInputParam();
            ccDataSlot.controller = ccCfg->getController();
            ccDataSlot.high_resolution = ccCfg->isHighResolution();
            ccDataSlot.scale = ccCfg->getScale();
            ccDataSlot.inverse = ccCfg->isParamInversed();
            ccDataSlot.slew_limiter.shape = ccCfg->getSlewLimiterShape();
            ccDataSlot.slew_limiter.rise = ccCfg->getSlewLimiterRise();
            ccDataSlot.slew_limiter.fall = ccCfg->getSlewLimiterFall();
            ccDataSlot.slew_limiter.sample_time = 1666.0f;
            ccDataSlot.zero_position = 0.0f;
        }
        catch (const std::exception& e){
            std::cerr << "Exception occured when updating data model with control change mapping: " << e.what() << std::endl;
        }
        
        sendActionMessage(GLOBAL_EVT_DATA_CC_CHANGED);
    }
    else if (auto* pbCfg = dynamic_cast<const MidiPitchBendConfig*>(&sysExCmd))
    {
        try {
            
            // Check boundaries of slots as underlying data structure
            // is based on C array, i.e. no exception is thrown when accessing
            // non available slot.
            if (pbCfg->getSlot() >= MAX_NUM_PB_MSG_PER_SENSOR){
                throw std::out_of_range("Can't access pitch bend slot " + std::to_string(pbCfg->getSlot()));
            }
            
            midi_pb_t& pbDataSlot = mappings.at(pbCfg->getSensorID()).pb_probs[pbCfg->getSlot()];
            
            pbDataSlot.enabled = pbCfg->isEnabled();
            pbDataSlot.channel = pbCfg->getChannel();
            pbDataSlot.input_param = pbCfg->getInputParam();
            pbDataSlot.scale = pbCfg->getScale();
            pbDataSlot.inverse = pbCfg->isParamInversed();
            pbDataSlot.unipolar = false;
        }
        catch (const std::exception& e){
            std::cerr << "Exception occured when updating data model with pitch bend mapping: " << e.what() << std::endl;
        }
        
        sendActionMessage(GLOBAL_EVT_DATA_PB_CHANGED);
    }
    else if (auto* batLevelMsg = dynamic_cast<const BatteryLevelConfig*>(&sysExCmd))
    {
        try {
            motion_sensor_t& sensor = mappings.at(batLevelMsg->getSensorId()).sensor;
            
            if (batLevelMsg->getBatteryLevel() <= 100)
                sensor.battery_level = batLevelMsg->getBatteryLevel();
            else
                sensor.battery_level = 0x7F;
        }
        catch (const std::exception& e){
            std::cerr << "Exception occured when updating data model with battery level: " << e.what() << std::endl;
        }
        
        sendActionMessage(GLOBAL_EVT_SENSOR_CHANGED);
    }
    else {
        std::wcerr << "Ignored SysEx message as not related to data model." << std::endl;
    }
}

void SomiDataModel::update(const juce::Component& component)
{
    if (auto* sensorHdrView = dynamic_cast<const SensorHeaderComponent*>(&component))
    {
        selectedSensor = sensorHdrView->getSelectedSensor();
        jassert(selectedSensor >= 0 && selectedSensor < MAX_NUM_SENSORS);
        
        sendActionMessage(GLOBAL_EVT_SENSOR_CHANGED);
        sendActionMessage(GLOBAL_EVT_DATA_CC_CHANGED);
        sendActionMessage(GLOBAL_EVT_DATA_NOTE_CHANGED);
        sendActionMessage(GLOBAL_EVT_DATA_PB_CHANGED);
    }
    else if (auto* ccHdrView = dynamic_cast<const ControlChangeHeaderComponent*>(&component))
    {
        // CC slot index corresponds to selected input movement parameter.
        selectedCcSlot = ccHdrView->getSelectedMovementParam();
        jassert(selectedCcSlot >= 0 && selectedCcSlot < MAX_NUM_CC_MSG_PER_SENSOR);

        mappings.at(selectedSensor).cc_probs[selectedCcSlot].input_param = static_cast<motion_param_t>(ccHdrView->getSelectedMovementParam());
        
        // Has to be sent as motion param also corresponds to slot
        sendActionMessage(GLOBAL_EVT_DATA_CC_CHANGED);
    }
    else if (auto* noteHdrView = dynamic_cast<const NotesHeaderComponent*>(&component))
    {
        // Currently only single note slot used
        selectedNoteSlot = 0;
        jassert(selectedNoteSlot >= 0 && selectedNoteSlot < MAX_NUM_NOTE_MSG_PER_SENSOR);

        mappings.at(selectedSensor).note_probs[selectedNoteSlot].input_param_gate = static_cast<motion_param_t>(noteHdrView->getSelectedMovementParam());
        
        // Used to update selected input parameter
        sendActionMessage(GLOBAL_EVT_DATA_NOTE_CHANGED);
    }
    else if (auto* pbHdrView = dynamic_cast<const PitchBendHeaderComponent*>(&component))
    {
        // Currently only single pitch bend slot used
        selectedPbSlot = 0;
        jassert(selectedPbSlot >= 0 && selectedPbSlot < MAX_NUM_PB_MSG_PER_SENSOR);
        
        mappings.at(selectedSensor).pb_probs[selectedPbSlot].input_param = static_cast<motion_param_t>(pbHdrView->getSelectedMovementParam());
        
        // Used to update selected input parameter
        sendActionMessage(GLOBAL_EVT_DATA_PB_CHANGED);
    }
    else if (auto* sensorView = dynamic_cast<const SensorComponent*>(&component))
    {
        // Nothing to do
    }
    else if (auto* ccView = dynamic_cast<const ControlChangeComponent*>(&component))
    {
        midi_cc_t& ccCfg = mappings.at(selectedSensor).cc_probs[selectedCcSlot];
        
        ccCfg.enabled = ccView->isActive();
        ccCfg.channel = ccView->getMidiChannel();
        //ccMapping.input_param = static_cast<motion_param_t>(selectedCcSlot);
        ccCfg.controller = ccView->getCcController();
        ccCfg.high_resolution = ccView->isHighResolution();
        ccCfg.scale = ccView->getScale();
        ccCfg.inverse = ccView->isInverse();
        //ccMapping.slew_limiter.sample_time = 1.0 / 1660.0;
        ccCfg.slew_limiter.shape = ccView->getSlewLimiterShape();
        ccCfg.slew_limiter.rise = ccView->getSlewLimiterRise();
        ccCfg.slew_limiter.fall = ccView->getSlewLimiterFall();
        //ccMapping.zero_position = 0.f;
    }
    else if (auto* noteView = dynamic_cast<const NotesComponent*>(&component))
    {
        midi_note_t& noteCfg = mappings.at(selectedSensor).note_probs[selectedNoteSlot];
        
        noteCfg.enabled = noteView->isActive();
        noteCfg.channel = noteView->getMidiChannel();
        //noteCfg.input_param_gate = static_cast<motion_param_t>(selectedNoteSlot);
        noteCfg.input_param_note = noteView->getParamNotePitch();
        noteCfg.note_min = noteView->getNoteMin();
        noteCfg.note_max = noteView->getNoteMax();
        noteCfg.velocity_note_on = 127;
        noteCfg.velocity_note_off = 0;
        noteCfg.threshold = noteView->getThreshold();
        noteCfg.inverse_gate_param = noteView->isGateInverse();
        //noteCfg.inverse_note_param = noteView->isPitchInverse();
    }
    else if (auto* pbView = dynamic_cast<const PitchBendComponent*>(&component))
    {
        midi_pb_t& pbCfg = mappings.at(selectedSensor).pb_probs[selectedPbSlot];
        
        pbCfg.enabled = pbView->isActive();
        pbCfg.channel = pbView->getMidiChannel();
        //pbCfg.input_param = static_cast<motion_param_t>(selectedPbSlot);
        pbCfg.scale = pbView->getScale();
        pbCfg.inverse = pbView->isInverse();
        pbCfg.unipolar = false;
    }
    else
    {
        std::wcerr << "Got unknown component to update data model!" << std::endl;
    }
}

int SomiDataModel::getSensorIdFromMidiMsg(const juce::MidiMessage& message) const
{
    for (int sensorId = 0; sensorId < MAX_NUM_SENSORS; sensorId++)
    {
        if (message.isController())
        {
            for (int slotIdx = 0; slotIdx < MAX_NUM_CC_MSG_PER_SENSOR; slotIdx++)
            {
                const midi_cc_t& ccCfg = mappings.at(sensorId).cc_probs[slotIdx];
                
                if (message.getChannel() - 1 == ccCfg.channel && message.getControllerNumber() == ccCfg.controller)
                {
                    return sensorId;
                }
            }
        }
        else {
            std::wcerr << "Can't get sensor ID of following MIDI message type: " << message.getDescription() << std::endl;
        }
    }
    
    return -1;
}

std::optional<motion_param_t> SomiDataModel::getParamFromMidiMsg(const juce::MidiMessage& message) const
{
    for (int sensorId = 0; sensorId < MAX_NUM_SENSORS; sensorId++)
    {
        if (message.isController())
        {
            for (int slotIdx = 0; slotIdx < MAX_NUM_CC_MSG_PER_SENSOR; slotIdx++)
            {
                const midi_cc_t& ccCfg = mappings.at(sensorId).cc_probs[slotIdx];
                
                if (message.getChannel() - 1 == ccCfg.channel &&
                    message.getControllerNumber() == ccCfg.controller)
                {
                    return ccCfg.input_param;
                }
            }
        }

        // TODO: Implement all supported MIDI messages
    }
    
    return std::nullopt;
}

// Global getters
int SomiDataModel::getSelectedSensor() const { return selectedSensor; }
int SomiDataModel::getSelectedNoteSlot() const { return selectedNoteSlot; }
int SomiDataModel::getSelectedCcSlot() const { return selectedCcSlot; }
int SomiDataModel::getSelectedPbSlot() const { return selectedPbSlot; }
int SomiDataModel::getBatteryLevel(int sensorId) const
{
    assertArgs(sensorId, 0, NOTE); // Just check sensor boundaries
    return mappings.at(sensorId).sensor.battery_level;
}
bool SomiDataModel::isSensorConnected(int sensorId) const
{
    assertArgs(sensorId, 0, NOTE); // Just check sensor boundaries
    return mappings.at(sensorId).sensor.battery_level <= 100;
}
bool SomiDataModel::isSomiHubAvaiable() const { return somiHubFound; }
void SomiDataModel::setSomiHubAvailable(bool found)
{
    somiHubFound = found;
    
    // Reset battery levels as they are used to indicate connection to sensor
    if (!found){
        for (int sensorId=0; sensorId < MAX_NUM_SENSORS; sensorId++){
            mappings.at(sensorId).sensor.battery_level = 0x7F;
        }
    }
    
    sendActionMessage(GLOBAL_EVT_SENSOR_CHANGED);
}

// Note getters
bool SomiDataModel::isNoteSlotEnabled(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].enabled;
}
int SomiDataModel::getNoteChannel(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].channel;
}
motion_param_t SomiDataModel::getNoteParamGate(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].input_param_gate;
}
motion_param_t SomiDataModel::getNoteParamNote(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].input_param_note;
}
int SomiDataModel::getNoteMin(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].note_min;
}
int SomiDataModel::getNoteMax(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].note_max;
}
int SomiDataModel::getNoteVelocityOn(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].velocity_note_on;
}
int SomiDataModel::getNoteVelocityOff(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].velocity_note_off;
}
float SomiDataModel::getNoteThreshold(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].threshold;
}
bool SomiDataModel::isNoteGateInversed(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].inverse_gate_param;
}
bool SomiDataModel::isNotePitchInversed(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return mappings.at(sensorId).note_probs[slot].inverse_note_param;
}
MidiNoteConfig SomiDataModel::getNoteCfg(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, NOTE);
    return MidiNoteConfig(sensorId,
                          mappings.at(sensorId).note_probs[slot].enabled,
                          slot,
                          mappings.at(sensorId).note_probs[slot].channel,
                          mappings.at(sensorId).note_probs[slot].input_param_gate,
                          mappings.at(sensorId).note_probs[slot].input_param_note,
                          mappings.at(sensorId).note_probs[slot].note_min,
                          mappings.at(sensorId).note_probs[slot].note_max,
                          mappings.at(sensorId).note_probs[slot].velocity_note_on,
                          mappings.at(sensorId).note_probs[slot].velocity_note_off,
                          mappings.at(sensorId).note_probs[slot].threshold,
                          mappings.at(sensorId).note_probs[slot].inverse_gate_param,
                          mappings.at(sensorId).note_probs[slot].inverse_note_param);
}

// Control change getters
bool SomiDataModel::isCcSlotEnabled(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].enabled;
}
int SomiDataModel::getCcChannel(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].channel;
}
motion_param_t SomiDataModel::getCcParam(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].input_param;
}
int SomiDataModel::getCcController(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].controller;
}
bool SomiDataModel::isCcHighResolution(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].high_resolution;
}
float SomiDataModel::getCcScale(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].scale;
}
bool SomiDataModel::isCcInversed(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].inverse;
}
float SomiDataModel::getCcSlewLimiterShape(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].slew_limiter.shape;
}
float SomiDataModel::getCcSlewLimiterRise(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].slew_limiter.rise;
}
float SomiDataModel::getCcSlewLimiterFall(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return mappings.at(sensorId).cc_probs[slot].slew_limiter.fall;
}
//float SomiDataModel::getCcZeroPosition(int sensorId, int slot) const; // Future use
MidiControlChangeConfig SomiDataModel::getCcCfg(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, CC);
    return MidiControlChangeConfig(sensorId,
                                   mappings.at(sensorId).cc_probs[slot].enabled,
                                   slot,
                                   mappings.at(sensorId).cc_probs[slot].channel,
                                   mappings.at(sensorId).cc_probs[slot].input_param,
                                   mappings.at(sensorId).cc_probs[slot].controller,
                                   mappings.at(sensorId).cc_probs[slot].scale,
                                   mappings.at(sensorId).cc_probs[slot].slew_limiter.shape,
                                   mappings.at(sensorId).cc_probs[slot].slew_limiter.rise,
                                   mappings.at(sensorId).cc_probs[slot].slew_limiter.fall,
                                   mappings.at(sensorId).cc_probs[slot].high_resolution,
                                   mappings.at(sensorId).cc_probs[slot].inverse);
}

// Pitch bend getters
bool SomiDataModel::isPbSlotEnabled(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, PITCHBEND);
    return mappings.at(sensorId).pb_probs[slot].enabled;
}
int SomiDataModel::getPbChannel(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, PITCHBEND);
    return mappings.at(sensorId).pb_probs[slot].channel;
}
motion_param_t SomiDataModel::getPbParam(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, PITCHBEND);
    return mappings.at(sensorId).pb_probs[slot].input_param;
}
float SomiDataModel::getPbScale(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, PITCHBEND);
    return mappings.at(sensorId).pb_probs[slot].scale;
}
bool SomiDataModel::isPbInversed(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, PITCHBEND);
    return mappings.at(sensorId).pb_probs[slot].inverse;
}
//bool SomiDataModel::isPbUnipolar(int sensorId, int slot) const; // Future use
MidiPitchBendConfig SomiDataModel::getPbCfg(int sensorId, int slot) const
{
    assertArgs(sensorId, slot, PITCHBEND);
    return MidiPitchBendConfig(sensorId,
                               mappings.at(sensorId).pb_probs[slot].enabled,
                               slot,
                               mappings.at(sensorId).pb_probs[slot].channel,
                               mappings.at(sensorId).pb_probs[slot].input_param,
                               mappings.at(sensorId).pb_probs[slot].scale,
                               mappings.at(sensorId).pb_probs[slot].inverse);
}

void SomiDataModel::assertArgs(int sensorId, int slot, midi_out_func_t outFunc)
{
    jassert(sensorId >= 0 && sensorId < MAX_NUM_SENSORS);
    
    switch(outFunc){
        case CC:
            jassert(slot >= 0 && slot < MAX_NUM_CC_MSG_PER_SENSOR);
            break;
        case NOTE:
            jassert(slot >= 0 && slot < MAX_NUM_NOTE_MSG_PER_SENSOR);
            break;
        case PITCHBEND:
            jassert(slot >= 0 && slot < MAX_NUM_PB_MSG_PER_SENSOR);
            break;
    }
}
