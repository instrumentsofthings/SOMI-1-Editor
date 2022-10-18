/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include "data_types.h"

#define SOMI_EDITOR_INFO_TEXT         "SOMI-1 Desktop Editor\n\n"\
                                      "\xC2\xA9 2022 Instruments of Things GmbH\n"\
                                      "All rights reserved\n\n"\
                                      "Development: Henrik Langer\n"\
                                      "GUI Design: David Knop\n\n"\
                                      "License: GPLv3\n"\
                                      "Credits: JUCE"

#define SOMI_URL_UPDATE               "https://instrumentsofthings.com/pages/support"
#define SOMI_URL_MANUAL               "https://cdn.shopify.com/s/files/1/0261/8310/6635/files/SOMI-1_Manual.pdf"
#define SOMI_URL_QUICK_START          "https://instrumentsofthings.com/pages/somi-1"
#define SOMI_URL_SUPPORT              "https://instrumentsofthings.com/pages/support"

#define MAX_NUM_SENSORS               6
#define TIMER_INTERVAL_MS             1000
#define UI_ACTIVITY_SCALE             8.0f

// Processing parameters
#define PARAM_RANGE_SCALE_MIN         0.0f
#define PARAM_RANGE_SCALE_MAX         10.f
#define MIDI_NOTE_MIDDLE_C            3 // Used to convert pitch to name

// Colors
#define SOMI_COLOR_BLUE               0xFF928CFF
#define SOMI_COLOR_YELLOW             0xFFE1FF5C

#define MIDI_SYSEX_MANF_ID_0          0x00
#define MIDI_SYSEX_MANF_ID_1          0x21
#define MIDI_SYSEX_MANF_ID_2          0x72
#define MIDI_SYSEX_DEV_ID             0x01 // SOMI-1

// Indices and SysEx message length defines
// don't take SysEx Start and Stop byte into account.
#define SYSEX_IDX_MANF_0              0                   // 1 byte
#define SYSEX_IDX_MANF_1              1                   // 1 byte
#define SYSEX_IDX_MANF_2              2                   // 1 byte
#define SYSEX_IDX_DEV_ID              3                   // 1 byte
#define SYSEX_IDX_SET_GET             4                   // 1 byte
#define SYSEX_IDX_CMD                 5                   // 1 int (5 bytes)
#define SYSEX_IDX_PARAMS_START        SYSEX_IDX_CMD + SYSEX_PARAM_LEN   // varying

#define SYSEX_FLAG_GET                0
#define SYSEX_FLAG_SET                1

#define SYSEX_PARAM_LEN               5 // bytes
#define SYSEX_HEADER_LEN             10 // bytes => manf (3 bytes) + dev id (byte) + set/get (byte) + cmd (int)
#define SYSEX_PARAM_MIDI_LEN          1 // bytes

#define SYSEX_CMD_CC_SOLO_LEN         2 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_CC_UNSOLO_LEN       0 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_NOTE_CFG_LEN       12 * SYSEX_PARAM_MIDI_LEN + 1 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_CC_CFG_LEN          8 * SYSEX_PARAM_MIDI_LEN + 4 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_PB_CFG_LEN          6 * SYSEX_PARAM_MIDI_LEN + 1 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_PERSIST_LEN         0 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_MAPPINGS_RESET_LEN  0 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_DISCOVER_LEN        0 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_SENSORS_RESET_LEN   0 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_DISABLE_SLOTS_LEN   0 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_BATTERY_LEVEL_LEN   2 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_SENSOR_STATE_LEN    2 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN
#define SYSEX_CMD_UPDATE_SAMD_LEN     0 * SYSEX_PARAM_MIDI_LEN + 0 * SYSEX_PARAM_LEN + SYSEX_HEADER_LEN

#define SYSEX_CFG_NOTE_LEN            SYSEX_CMD_NOTE_CFG_LEN - 2 - SYSEX_HEADER_LEN // Note config without sensor id and note index
#define SYSEX_CFG_CC_LEN              SYSEX_CMD_CC_CFG_LEN   - 2 - SYSEX_HEADER_LEN // CC config without sensor id and cc index
#define SYSEX_CFG_PB_LEN              SYSEX_CMD_PB_CFG_LEN   - 2 - SYSEX_HEADER_LEN // PB config without sensor id and pb index

// Global events
#define GLOBAL_EVT_INFO_BTN_CLICKED     "BTN_INFO_CLICKED"
#define GLOBAL_EVT_BTN_APPLY_CLICKED    "BTN_APPLY_CLICKED"
#define GLOBAL_EVT_BTN_SAVE_CLICKED     "BTN_SAVE_CLICKED"
#define GLOBAL_EVT_BTN_RST_SENSORS      "BTN_RST_SENSORS_CLICKED"
#define GLOBAL_EVT_BTN_RST_MAPPINGS     "BTN_RST_MAPPINGS_CLICKED"
#define GLOBAL_EVT_BTN_CC_SOLO          "BTN_CC_SOLO_CLICKED"
#define GLOBAL_EVT_BTN_CC_UNSOLO        "BTN_CC_UNSOLO_CLICKED"
#define GLOBAL_EVT_DATA_CC_CHANGED      "GLOBAL_EVT_DATA_CC_CHANGED"
#define GLOBAL_EVT_DATA_NOTE_CHANGED    "GLOBAL_EVT_DATA_NOTE_CHANGED"
#define GLOBAL_EVT_DATA_PB_CHANGED      "GLOBAL_EVT_DATA_PB_CHANGED"
#define GLOBAL_EVT_SENSOR_CHANGED       "GLOBAL_EVT_SENSOR_CHANGED"


enum SysExCmd {
    SYSEX_CMD_UNKNOWN = 0,
    SYSEX_CMD_CC_SOLO,        // Params: channel (byte), cc controller (byte)
    SYSEX_CMD_CC_UNSOLO,      // Params: void
    SYSEX_CMD_NOTE_CFG,       // Params: sensor id (byte), note index (byte), enabled (byte), channel (byte), input gate (byte), input note (byte), note min (byte), note max (byte), velocity note on (byte), velocity note off (byte), threshold (float), inverse gate (byte), inverse note (byte)
    SYSEX_CMD_CC_CFG,         // Params: sensor id (byte), cc index (byte), enabled (byte), channel (byte), input param (byte), CC controller (byte), high resolution (byte), scale (float), inverse (byte), slew shape (float), slew rise (float), slew fall (float)
    SYSEX_CMD_PB_CFG,         // Params: sensor id (byte), pb index (byte), enabled (byte), channel (byte), input param (byte), scale (float), inverse (byte)
    SYSEX_CMD_PERSIST,        // Params: void
    SYSEX_CMD_MAPPINGS_RESET, // Params: void
    SYSEX_CMD_DISCOVER,       // Params: void
    SYSEX_CMD_SENSOR_CFG,     // Params: sensor id (byte),
    SYSEX_CMD_SENSORS_RESET,  // Params: void
    SYSEX_CMD_DISABLE_SLOTS,  // Params: void
    SYSEX_CMD_BATTERY_LEVEL,  // Params: sensor id (byte), battery level (byte)
    SYSEX_CMD_SENSOR_STATE,   // Params: sensor id (byte), state (byte)
    SYSEX_CMD_UPDATE_SAMD     // Params: void
};
