/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include "Mapping.h"

#include "Utils.h"

#include <cerrno>
#include <iostream>

SysExCmdMessage::SysExCmdMessage(){}
SysExCmdMessage::~SysExCmdMessage(){}

SysExCmd SysExCmdMessage::getSysExCmd(const juce::MidiMessage &sysExMsg)
{        
    if (!sysExMsg.isSysEx() ||
        sysExMsg.getSysExDataSize() < SYSEX_HEADER_LEN ||
        sysExMsg.getSysExData()[SYSEX_IDX_MANF_0]   != MIDI_SYSEX_MANF_ID_0 ||
        sysExMsg.getSysExData()[SYSEX_IDX_MANF_1]   != MIDI_SYSEX_MANF_ID_1 ||
        sysExMsg.getSysExData()[SYSEX_IDX_MANF_2]   != MIDI_SYSEX_MANF_ID_2 ||
        (sysExMsg.getSysExData()[SYSEX_IDX_SET_GET] != SYSEX_FLAG_GET && sysExMsg.getSysExData()[SYSEX_IDX_SET_GET] != SYSEX_FLAG_SET))
    {
        return SYSEX_CMD_UNKNOWN;
    }
    
    int cmd = sysExToInt(sysExMsg.getSysExData() + SYSEX_IDX_CMD);
    
    if (cmd == SYSEX_CMD_CC_SOLO && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_CC_SOLO_LEN)
        return SYSEX_CMD_CC_SOLO;
    else if (cmd == SYSEX_CMD_CC_UNSOLO && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_CC_UNSOLO_LEN)
        return SYSEX_CMD_CC_UNSOLO;
    else if (cmd == SYSEX_CMD_NOTE_CFG && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_NOTE_CFG_LEN)
        return SYSEX_CMD_NOTE_CFG;
    else if (cmd == SYSEX_CMD_CC_CFG && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_CC_CFG_LEN)
        return SYSEX_CMD_CC_CFG;
    else if (cmd == SYSEX_CMD_PB_CFG && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_PB_CFG_LEN)
        return SYSEX_CMD_PB_CFG;
    else if (cmd == SYSEX_CMD_PERSIST && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_PERSIST_LEN)
        return SYSEX_CMD_PERSIST;
    else if (cmd == SYSEX_CMD_MAPPINGS_RESET && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_MAPPINGS_RESET_LEN)
        return SYSEX_CMD_MAPPINGS_RESET;
    else if (cmd == SYSEX_CMD_DISCOVER && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_DISCOVER_LEN)
        return SYSEX_CMD_DISCOVER;
    else if (cmd == SYSEX_CMD_SENSOR_CFG) // Future use
        return SYSEX_CMD_SENSOR_CFG;
    else if (cmd == SYSEX_CMD_SENSORS_RESET && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_SENSORS_RESET_LEN)
        return SYSEX_CMD_SENSORS_RESET;
    else if (cmd == SYSEX_CMD_DISABLE_SLOTS && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_DISABLE_SLOTS_LEN)
        return SYSEX_CMD_DISABLE_SLOTS;
    else if (cmd == SYSEX_CMD_BATTERY_LEVEL && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_BATTERY_LEVEL_LEN)
        return SYSEX_CMD_BATTERY_LEVEL;
    else if (cmd == SYSEX_CMD_SENSOR_STATE && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_SENSOR_STATE_LEN)
        return SYSEX_CMD_SENSOR_STATE;
    else if (cmd == SYSEX_CMD_UPDATE_SAMD && (sysExMsg.getSysExDataSize()) == SYSEX_CMD_UPDATE_SAMD_LEN)
        return SYSEX_CMD_UPDATE_SAMD;
    else
        return SYSEX_CMD_UNKNOWN;
}

void SysExCmdMessage::intToSysEx(int32_t val, uint8_t midi_buf[5])
{
    // TODO: Use for loop => more elegant and scaleable
    midi_buf[0] = (*((uint32_t*)&val) >> 28) & 0x0F;
    midi_buf[1] = (*((uint32_t*)&val) >> 21) & 0x7F;
    midi_buf[2] = (*((uint32_t*)&val) >> 14) & 0x7F;
    midi_buf[3] = (*((uint32_t*)&val) >>  7) & 0x7F;
    midi_buf[4] = (*((uint32_t*)&val) >>  0) & 0x7F;
}

int32_t SysExCmdMessage::sysExToInt(const uint8_t midi_buf[5])
{
    uint32_t ret = 0;
    
    // TODO: Use for loop => more elegant and scaleable
    ret |= ((uint32_t)(midi_buf[0] & 0x0F) << 28);
    ret |= ((uint32_t)(midi_buf[1] & 0x7F) << 21);
    ret |= ((uint32_t)(midi_buf[2] & 0x7F) << 14);
    ret |= ((uint32_t)(midi_buf[3] & 0x7F) <<  7);
    ret |= ((uint32_t)(midi_buf[4] & 0x7F) <<  0);
    
    return *((int32_t*)&ret);
}

void SysExCmdMessage::floatToSysex(float val, uint8_t midi_buf[5])
{
    // TODO: Use for loop => more elegant and scaleable
    midi_buf[0] = (*((uint32_t*)&val) >> 28) & 0x0F;
    midi_buf[1] = (*((uint32_t*)&val) >> 21) & 0x7F;
    midi_buf[2] = (*((uint32_t*)&val) >> 14) & 0x7F;
    midi_buf[3] = (*((uint32_t*)&val) >>  7) & 0x7F;
    midi_buf[4] = (*((uint32_t*)&val) >>  0) & 0x7F;
}

float SysExCmdMessage::sysexToFloat(const uint8_t midi_buf[5])
{
    uint32_t ret = 0.f;

    // TODO: Use for loop => more elegant and scaleable
    ret |= ((uint32_t)(midi_buf[0] & 0x0F) << 28);
    ret |= ((uint32_t)(midi_buf[1] & 0x7F) << 21);
    ret |= ((uint32_t)(midi_buf[2] & 0x7F) << 14);
    ret |= ((uint32_t)(midi_buf[3] & 0x7F) <<  7);
    ret |= ((uint32_t)(midi_buf[4] & 0x7F) <<  0);

    return *((float*)&ret);
}


CCSoloConfig::CCSoloConfig(uint8_t channel, uint8_t controller) :
soloed(false)
{
    this->channel = channel & 0xF;
    this->controller = controller & 0x7F;
}

CCSoloConfig::~CCSoloConfig(){}

juce::MidiMessage CCSoloConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    
    if (soloed){
        intToSysEx(SYSEX_CMD_CC_SOLO, cmdBuf);
        
        // Excluding SysEx Start and Stop byte
        uint8_t sysex_buf[] = {
            MIDI_SYSEX_MANF_ID_0,
            MIDI_SYSEX_MANF_ID_1,
            MIDI_SYSEX_MANF_ID_2,
            MIDI_SYSEX_DEV_ID,
            SYSEX_FLAG_SET,
            cmdBuf[0],
            cmdBuf[1],
            cmdBuf[2],
            cmdBuf[3],
            cmdBuf[4],
            static_cast<uint8_t>(channel & 0x7F),
            static_cast<uint8_t>(controller & 0x7F)
        };
        
        return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
    }
    else{
        intToSysEx(SYSEX_CMD_CC_UNSOLO, cmdBuf);
        
        // Excluding SysEx Start and Stop byte
        uint8_t sysex_buf[] = {
            MIDI_SYSEX_MANF_ID_0,
            MIDI_SYSEX_MANF_ID_1,
            MIDI_SYSEX_MANF_ID_2,
            MIDI_SYSEX_DEV_ID,
            SYSEX_FLAG_SET,
            cmdBuf[0],
            cmdBuf[1],
            cmdBuf[2],
            cmdBuf[3],
            cmdBuf[4],
        };
        
        return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
    }
}

juce::String CCSoloConfig::getDescription() const
{
    return "SysEx cmd cc solo/unsolo";
}

void CCSoloConfig::setSolo(bool state)
{
    soloed = state;
}
bool CCSoloConfig::isSoloed() const { return soloed; }

void CCSoloConfig::setChannel(uint8_t channel)
{
    this->channel = channel & 0xF;
}
int CCSoloConfig::getChannel() const { return channel; }

void CCSoloConfig::setController(uint8_t controller)
{
    this->controller = controller & 0x7F;
}
int CCSoloConfig::getController() const { return controller; }


juce::MidiMessage PersistConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_PERSIST, cmdBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        SYSEX_FLAG_SET,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String PersistConfig::getDescription() const
{
    return "SysEx persist command";
}


juce::MidiMessage ResetMappingsConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_MAPPINGS_RESET, cmdBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        SYSEX_FLAG_SET,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String ResetMappingsConfig::getDescription() const
{
    return "SysEx reset mappings command";
}


juce::MidiMessage ResetSensorsConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_SENSORS_RESET, cmdBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        SYSEX_FLAG_SET,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String ResetSensorsConfig::getDescription() const
{
    return "SysEx reset sensors command";
}


juce::MidiMessage DiscoverConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_DISCOVER, cmdBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        SYSEX_FLAG_GET,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String DiscoverConfig::getDescription() const
{
    return "SysEx discover command";
}

BatteryLevelConfig::BatteryLevelConfig(uint8_t sensorID, uint8_t batteryLevel) : sensorID(sensorID), batteryLevel(batteryLevel){}

BatteryLevelConfig::BatteryLevelConfig(const juce::MidiMessage& sysExMsg)
{
    if (!sysExMsg.isSysEx() || sysExMsg.getSysExDataSize() != SYSEX_CMD_BATTERY_LEVEL_LEN)
    {
        std::cerr << "Got wrong MIDI message in BatteryLevelConfig constructor!" << std::endl;
        
        // Use default values
        sensorID = 0;
        batteryLevel = 0x7F; // indicates error
    }
    else {
        sensorID        = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 0];
        batteryLevel    = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 1];
    }
}

BatteryLevelConfig::~BatteryLevelConfig(){}

juce::MidiMessage BatteryLevelConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_BATTERY_LEVEL, cmdBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        SYSEX_FLAG_GET,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
        static_cast<uint8_t>(sensorID & 0x7F),
        static_cast<uint8_t>(batteryLevel & 0x7F)
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String BatteryLevelConfig::getDescription() const
{
    if (isSensorConnected()){
        return "Sensor " + juce::String(sensorID) + " has battery level " + juce::String(batteryLevel) + "%.";
    }
    else{
        return "Sensor " + juce::String(sensorID) + " is not connected.";
    }
}

bool BatteryLevelConfig::isSensorConnected() const
{
    if (batteryLevel == 0x7F)
        return false;
    else
        return true;
}

void BatteryLevelConfig::setSensorId(uint8_t sensorID)
{
    this->sensorID = sensorID;
}
int BatteryLevelConfig::getSensorId() const { return sensorID; }

void BatteryLevelConfig::setBatteryLevel(uint8_t batteryLevel)
{
    this->batteryLevel = batteryLevel;
}

int BatteryLevelConfig::getBatteryLevel() const { return batteryLevel; }

juce::MidiMessage DisableSlotsConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_DISABLE_SLOTS, cmdBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        SYSEX_FLAG_GET,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}


MidiConfig::MidiConfig(uint8_t sensorID,
                       bool enabled,
                       uint8_t slot,
                       uint8_t channel) :
sensorID(sensorID),
enabled(enabled),
slot(slot),
channel(channel){}

MidiConfig::MidiConfig(const juce::String& jsonString)
{
    juce::var noteConfigObj;
    juce::Result res = juce::JSON::parse(jsonString, noteConfigObj);
    if (res.failed()){
        std::cerr << res.getErrorMessage().toStdString() << std::endl;
    }
    
    sensorID = static_cast<int>(noteConfigObj.getProperty("SensorId", 0)) & 0x7F;
    slot     = static_cast<int>(noteConfigObj.getProperty("SlotIndex", 0)) & 0x7F;
    enabled  = static_cast<bool>(noteConfigObj.getProperty("Enable", false)) & 0x1;
    channel  = static_cast<int>(noteConfigObj.getProperty("MidiChannel", 0)) & 0xF;
}

MidiConfig::MidiConfig(const juce::MidiMessage& sysExMsg)
{
    if (!sysExMsg.isSysEx() ||
        sysExMsg.getSysExDataSize() < SYSEX_HEADER_LEN)
    {
        std::cerr << "Failed creating MIDI config from SysEx message!" << std::endl;
        
        // Using default values
        sensorID = 0;
        slot = 0;
        enabled = false;
        channel = 0;
    }
    else {
        sensorID         = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 0];
        slot             = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 1];
        enabled          = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 2] & 0x1;
        channel          = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 3];
    }
}

MidiConfig::~MidiConfig(){}

int MidiConfig::getSensorID() const { return sensorID; }
bool MidiConfig::isEnabled() const { return enabled; }
/*
MidiConfig& MidiConfig::setEnabled(bool enable)
{
    this->enabled = enable;
    return *this;
}
*/
int MidiConfig::getSlot() const { return slot; }
int MidiConfig::getChannel() const { return channel; }


MidiNoteConfig::MidiNoteConfig(uint8_t sensorID,
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
                               bool inverseNoteParam) :
MidiConfig(sensorID, enabled, slot, channel),
inputParamGate(inputParamGate),
inputParamNote(inputParamNote),
noteMin(noteMin),
noteMax(noteMax),
velocityNoteOn(velocityNoteOn),
velocityNoteOff(velocityNoteOff),
threshold(threshold),
inverseGateParam(inverseGateParam),
inverseNoteParam(inverseNoteParam){}

MidiNoteConfig::MidiNoteConfig(const juce::String& jsonString) :
MidiConfig(jsonString)
{
    juce::var noteConfigObj;
    juce::Result res = juce::JSON::parse(jsonString, noteConfigObj);
    if (res.failed()){
        std::cerr << res.getErrorMessage().toStdString() << std::endl;
    }
    
    inputParamGate = static_cast<motion_param_t>(static_cast<int>(noteConfigObj.getProperty("InputParamGate", 0)));
    inputParamNote = static_cast<motion_param_t>(static_cast<int>(noteConfigObj.getProperty("InputParamNote", 0)));
    noteMin = static_cast<int>(noteConfigObj.getProperty("NoteMin", 0)) & 0x7F;
    noteMax = static_cast<int>(noteConfigObj.getProperty("NoteMax", 0)) & 0x7F;
    velocityNoteOn = static_cast<int>(noteConfigObj.getProperty("VelocityNoteOn", 0)) & 0x7F;
    velocityNoteOff = static_cast<int>(noteConfigObj.getProperty("VelocityNoteOff", 0)) & 0x7F;
    threshold = static_cast<float>(noteConfigObj.getProperty("GateThreshold", 0));
    inverseGateParam  = static_cast<bool>(noteConfigObj.getProperty("InverseParamGate", false)) & 0x1;
    inverseNoteParam  = static_cast<bool>(noteConfigObj.getProperty("InverseParamNote", false)) & 0x1;
}

MidiNoteConfig::MidiNoteConfig(const juce::MidiMessage& sysExMsg) :
MidiConfig(sysExMsg)
{
    if (!sysExMsg.isSysEx() ||
        sysExMsg.getSysExDataSize() != SYSEX_CMD_NOTE_CFG_LEN)
    {
        std::cerr << "Failed creating MIDI note config from SysEx message!" << std::endl;
        
        // using default values
        inputParamGate = TILT_X;
        inputParamNote = TILT_Z;
        noteMin = 36;
        noteMax = 60;
        velocityNoteOn = 127;
        velocityNoteOff = 0;
        threshold = 0.5f;
        inverseGateParam = false;
        inverseNoteParam = false;
    }
    else {
        inputParamGate   = static_cast<motion_param_t>(sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 4]);
        inputParamNote   = static_cast<motion_param_t>(sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 5]);
        noteMin          = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 6];
        noteMax          = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 7];
        velocityNoteOn   = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 8];
        velocityNoteOff  = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 9];
        threshold        = sysexToFloat(sysExMsg.getSysExData() + SYSEX_IDX_PARAMS_START + 10);
        inverseGateParam = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 15];
        inverseNoteParam = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 16];
    }
}

MidiNoteConfig::~MidiNoteConfig(){}

juce::MidiMessage MidiNoteConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_NOTE_CFG, cmdBuf);
    
    uint8_t thresholdBuf[5];
    floatToSysex(threshold, thresholdBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        !request,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
        static_cast<uint8_t>(sensorID & 0x7F),
        static_cast<uint8_t>(slot & 0x7F),
        static_cast<uint8_t>(enabled & 0x1),
        static_cast<uint8_t>(channel & 0xF),
        static_cast<uint8_t>(inputParamGate),
        static_cast<uint8_t>(inputParamNote),
        static_cast<uint8_t>(noteMin & 0x7F),
        static_cast<uint8_t>(noteMax & 0x7F),
        static_cast<uint8_t>(velocityNoteOn & 0x7F),
        static_cast<uint8_t>(velocityNoteOff & 0x7F),
        thresholdBuf[0],
        thresholdBuf[1],
        thresholdBuf[2],
        thresholdBuf[3],
        thresholdBuf[4],
        static_cast<uint8_t>(inverseGateParam),
        static_cast<uint8_t>(inverseNoteParam),
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String MidiNoteConfig::getDescription() const
{
    return juce::String("Note Configuration:\n")
        + "input param gate: " + juce::String(inputParamGate)
        + "input param note: " + juce::String(inputParamNote)
        + "note min: " + juce::String(noteMin)
        + "note max: " + juce::String(noteMax)
        + "velocity note on: " + juce::String(velocityNoteOn)
        + "velocity note off: " + juce::String(velocityNoteOff)
        + "threshold: " + juce::String(threshold)
        + "inversed gate param: " + Utils::btos(inverseGateParam)
        + "inversed note param: " + Utils::btos(inverseNoteParam);
}

motion_param_t MidiNoteConfig::getInputParamGate() const { return inputParamGate; }
motion_param_t MidiNoteConfig::getInputParamNote() const { return inputParamNote; }
int MidiNoteConfig::getNoteMin() const { return noteMin; }
int MidiNoteConfig::getNoteMax() const { return noteMax; }
int MidiNoteConfig::getVelocityOn() const { return velocityNoteOn; }
int MidiNoteConfig::getVelocityOff() const { return velocityNoteOff; }
float MidiNoteConfig::getThreshold() const { return threshold; }
bool MidiNoteConfig::isParamGateInversed() const { return inverseGateParam; }
bool MidiNoteConfig::isParamNoteInversed() const { return inverseNoteParam; }


MidiControlChangeConfig::MidiControlChangeConfig(uint8_t sensorID,
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
                                           bool inverseParam) :
MidiConfig(sensorID, enabled, slot, channel),
inputParam(inputParam),
controller(controller),
highResolution(highResolution),
scale(scale),
inverseParam(inverseParam),
slewLimiterShape(slewLimiterShape),
slewLimiterRise(slewLimiterRise),
slewLimiterFall(slewLimiterFall){}

MidiControlChangeConfig::MidiControlChangeConfig(const juce::String& jsonString) :
MidiConfig(jsonString)
{
    juce::var ccConfigObj;
    juce::Result res = juce::JSON::parse(jsonString, ccConfigObj);
    if (res.failed()){
        std::cerr << res.getErrorMessage().toStdString() << std::endl;
    }
    
    inputParam = static_cast<motion_param_t>(static_cast<int>(ccConfigObj.getProperty("InputParam", 0)));
    controller = static_cast<int>(ccConfigObj.getProperty("Controller", 0)) & 0x7F;
    highResolution  = static_cast<bool>(ccConfigObj.getProperty("HighResolution", false)) & 0x1;
    scale = static_cast<float>(ccConfigObj.getProperty("Scale", 0));
    slewLimiterShape = static_cast<float>(ccConfigObj.getProperty("SlewLimiterShape", 0));
    slewLimiterRise = static_cast<float>(ccConfigObj.getProperty("SlewLimiterRise", 0));
    slewLimiterFall = static_cast<float>(ccConfigObj.getProperty("SlewLimiterFall", 0));
    inverseParam = static_cast<bool>(ccConfigObj.getProperty("InverseInputParam", false)) & 0x1;
}

MidiControlChangeConfig::MidiControlChangeConfig(const juce::MidiMessage& sysExMsg) :
MidiConfig(sysExMsg)
{
    if (!sysExMsg.isSysEx() ||
        sysExMsg.getSysExDataSize() != SYSEX_CMD_CC_CFG_LEN)
    {
        std::cerr << "Failed creating MIDI CC config from SysEx message!" << std::endl;
        
        // Use default values
        inputParam = TILT_X;
        controller = 16;
        highResolution = false;
        scale = 1.0f;
        inverseParam = false;
        slewLimiterShape = 0.5f;
        slewLimiterRise = 0.0f;
        slewLimiterRise = 0.0f;
    }
    else {
        inputParam       = static_cast<motion_param_t>(sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 4]);
        controller       = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 5];
        highResolution   = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 6];
        scale            = sysexToFloat(sysExMsg.getSysExData() + SYSEX_IDX_PARAMS_START + 7);
        inverseParam     = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 12];
        slewLimiterShape = sysexToFloat(sysExMsg.getSysExData() + SYSEX_IDX_PARAMS_START + 13);
        slewLimiterRise  = sysexToFloat(sysExMsg.getSysExData() + SYSEX_IDX_PARAMS_START + 18);
        slewLimiterFall  = sysexToFloat(sysExMsg.getSysExData() + SYSEX_IDX_PARAMS_START + 23);
    }
}

MidiControlChangeConfig::~MidiControlChangeConfig(){}

juce::MidiMessage MidiControlChangeConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_CC_CFG, cmdBuf);
    
    uint8_t scaleBuf[5], slewShapeBuf[5], slewRiseBuf[5], slewFallBuf[5];
    floatToSysex(scale, scaleBuf);
    floatToSysex(slewLimiterShape, slewShapeBuf);
    floatToSysex(slewLimiterRise, slewRiseBuf);
    floatToSysex(slewLimiterFall, slewFallBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        !request,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
        static_cast<uint8_t>(sensorID & 0x7F),
        static_cast<uint8_t>(slot & 0x7F),
        static_cast<uint8_t>(enabled & 0x1),
        static_cast<uint8_t>(channel & 0xF),
        static_cast<uint8_t>(inputParam),
        static_cast<uint8_t>(controller & 0x7F),
        static_cast<uint8_t>(highResolution & 0x1),
        scaleBuf[0],
        scaleBuf[1],
        scaleBuf[2],
        scaleBuf[3],
        scaleBuf[4],
        static_cast<uint8_t>(inverseParam & 0x1),
        slewShapeBuf[0],
        slewShapeBuf[1],
        slewShapeBuf[2],
        slewShapeBuf[3],
        slewShapeBuf[4],
        slewRiseBuf[0],
        slewRiseBuf[1],
        slewRiseBuf[2],
        slewRiseBuf[3],
        slewRiseBuf[4],
        slewFallBuf[0],
        slewFallBuf[1],
        slewFallBuf[2],
        slewFallBuf[3],
        slewFallBuf[4],
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String MidiControlChangeConfig::getDescription() const
{
    return juce::String("Control Change Configuration:\n")
        + "input param: " + juce::String(inputParam)
        + "controller: " + juce::String(controller)
        + "14 bit: " + Utils::btos(highResolution)
        + "scale: " + juce::String(scale)
        + "inversed param: " + Utils::btos(inverseParam)
        + "slew limiter shape: " + juce::String(slewLimiterShape * 100.0f) + "%"
        + "slew limiter rise: " + juce::String(slewLimiterRise)
        + "slew limiter fall: " + juce::String(slewLimiterFall);
}

motion_param_t MidiControlChangeConfig::getInputParam() const { return inputParam; }
int MidiControlChangeConfig::getController() const { return controller; }
bool MidiControlChangeConfig::isHighResolution() const { return highResolution; }
float MidiControlChangeConfig::getScale() const { return scale; }
bool MidiControlChangeConfig::isParamInversed() const { return inverseParam; }
float MidiControlChangeConfig::getSlewLimiterShape() const { return slewLimiterShape; }
float MidiControlChangeConfig::getSlewLimiterRise() const { return slewLimiterRise; }
float MidiControlChangeConfig::getSlewLimiterFall() const { return slewLimiterFall; }
/*
MidiControlChangeConfig& MidiControlChangeConfig::setHighResolution(bool highResolution)
{
    this->highResolution = highResolution;
    return *this;
}
*/


MidiPitchBendConfig::MidiPitchBendConfig(uint8_t sensorID,
                                   bool enabled,
                                   uint8_t slot,
                                   uint8_t channel,
                                   motion_param_t inputParam,
                                   float scale,
                                   bool inverseParam) :
MidiConfig(sensorID, enabled, slot, channel),
inputParam(inputParam),
scale(scale),
inverseParam(inverseParam){}

MidiPitchBendConfig::MidiPitchBendConfig(const juce::String& jsonString) :
MidiConfig(jsonString)
{
    juce::var pbConfigObj;
    juce::Result res = juce::JSON::parse(jsonString, pbConfigObj);
    if (res.failed()){
        std::cerr << res.getErrorMessage().toStdString() << std::endl;
    }
    
    inputParam = static_cast<motion_param_t>(static_cast<int>(pbConfigObj.getProperty("InputParam", 0)));
    scale = static_cast<float>(pbConfigObj.getProperty("Scale", 0));
    inverseParam = static_cast<bool>(pbConfigObj.getProperty("InverseInputParam", false)) & 0x1;
}

MidiPitchBendConfig::MidiPitchBendConfig(const juce::MidiMessage& sysExMsg) :
MidiConfig(sysExMsg)
{
    if (!sysExMsg.isSysEx() ||
        sysExMsg.getSysExDataSize() != SYSEX_CMD_PB_CFG_LEN)
    {
        std::cerr << "Failed creating MIDI PB config from SysEx message!" << std::endl;
        
        // Using default values
        inputParam = ACC_X;
        scale = 1.0f;
        inverseParam = false;
    }
    else {
        inputParam = static_cast<motion_param_t>(sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 4]);
        scale = sysexToFloat(sysExMsg.getSysExData() + SYSEX_IDX_PARAMS_START + 5);
        inverseParam = sysExMsg.getSysExData()[SYSEX_IDX_PARAMS_START + 10];
    }
}

MidiPitchBendConfig::~MidiPitchBendConfig(){}

juce::MidiMessage MidiPitchBendConfig::createSysExMessage(bool request) const
{
    uint8_t cmdBuf[5] = {0};
    intToSysEx(SYSEX_CMD_PB_CFG, cmdBuf);
    
    uint8_t scaleBuf[5];
    floatToSysex(scale, scaleBuf);
    
    // Excluding SysEx Start and Stop byte
    uint8_t sysex_buf[] = {
        MIDI_SYSEX_MANF_ID_0,
        MIDI_SYSEX_MANF_ID_1,
        MIDI_SYSEX_MANF_ID_2,
        MIDI_SYSEX_DEV_ID,
        !request,
        cmdBuf[0],
        cmdBuf[1],
        cmdBuf[2],
        cmdBuf[3],
        cmdBuf[4],
        static_cast<uint8_t>(sensorID & 0x7F),
        static_cast<uint8_t>(slot & 0x7F),
        static_cast<uint8_t>(enabled & 0x1),
        static_cast<uint8_t>(channel & 0xF),
        static_cast<uint8_t>(inputParam),
        scaleBuf[0],
        scaleBuf[1],
        scaleBuf[2],
        scaleBuf[3],
        scaleBuf[4],
        static_cast<uint8_t>(inverseParam & 0x1),
    };
    
    return juce::MidiMessage::createSysExMessage(sysex_buf, sizeof(sysex_buf));
}

juce::String MidiPitchBendConfig::getDescription() const
{
    return juce::String("Pitch Bend Configuration:\n")
        + "input param: " + juce::String(inputParam)
        + "scale: " + juce::String(scale)
        + "inversed param: " + Utils::btos(inverseParam);
}

motion_param_t MidiPitchBendConfig::getInputParam() const { return inputParam; }
float MidiPitchBendConfig::getScale() const { return scale; }
bool MidiPitchBendConfig::isParamInversed() const { return inverseParam; }
