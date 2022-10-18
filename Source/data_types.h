/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#ifndef DATA_TYPES_H__
#define DATA_TYPES_H__

#define NUM_MOTION_PARAMS                                 7
#define MOTION_SENSOR_MAX_NAME_LEN                       32
#define MAX_NUM_NOTE_MSG_PER_SENSOR                       4
#define MAX_NUM_CC_MSG_PER_SENSOR                         8
#define MAX_NUM_PB_MSG_PER_SENSOR                         2

#include <stdint.h>
#include <stdbool.h>

/**
 * Enum representing motion parameters
 */
typedef enum motion_param {
    TILT_X = 0,
    TILT_Y,
    TILT_Z,
    ACC_X,
    ACC_Y,
    ACC_Z,
    ACTIVITY,
} motion_param_t;

/**
 * Enum representing type of MIDI message
 */
typedef enum midi_out_func {
    CC = 0,
    NOTE,
    PITCHBEND
} midi_out_func_t;

/**
 * Structure representing slew limiter
 */
typedef struct {
    float shape;
    float rise;
    float fall;
    float sample_time;
} slew_limiter_t;

/**
 * Structure representing motion sensor
 */
typedef struct {
    int16_t         ID; // unique, fixed ID. Starts from zero (negative value indicates that sensor is not known)
    int16_t         conn_id; // varies!
    uint8_t         address[6];
    char            name[MOTION_SENSOR_MAX_NAME_LEN];
    uint16_t        name_handle;
    uint8_t         battery_level;
    uint16_t        battery_handle;
    bool            roll_360;   // Future use
    bool            pitch_360;  // Future use
    bool            yaw_360;    // Future use
} motion_sensor_t;

/**
 * Structure representing MIDI note configuration
 */
typedef struct {
    bool            enabled;
    uint8_t         channel;
    motion_param_t  input_param_gate;
    motion_param_t  input_param_note;
    uint8_t         note_min;
    uint8_t         note_max;
    uint8_t         velocity_note_on;
    uint8_t         velocity_note_off;
    float           threshold;
    bool            inverse_gate_param;
    bool            inverse_note_param;
} midi_note_t;

/**
 * Structure representing MIDI CC configuration
 */
typedef struct {
    bool            enabled;
    uint8_t         channel;
    motion_param_t  input_param;
    uint8_t         controller;
    bool            high_resolution;
    float           scale;
    bool            inverse;
    slew_limiter_t  slew_limiter;
    float           zero_position; // Future use
} midi_cc_t;

/**
 * Structure representing MIDI pitchbend configuration
 */
typedef struct {
    bool            enabled;
    uint8_t         channel;
    motion_param_t  input_param;
    float           scale;
    bool            inverse;
    bool            unipolar; // Future use
} midi_pb_t;

/**
 * Structure representing output mapping with different attributes
 */
typedef struct {
    motion_sensor_t  sensor;
    midi_note_t      note_probs[MAX_NUM_NOTE_MSG_PER_SENSOR];
    midi_cc_t        cc_probs[MAX_NUM_CC_MSG_PER_SENSOR];
    midi_pb_t        pb_probs[MAX_NUM_PB_MSG_PER_SENSOR];
} output_mapping_t;

#endif // DATA_TYPES_H__
