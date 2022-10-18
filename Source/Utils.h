/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#pragma once

#include "data_types.h"

class Utils
{
public:
    
    // Remaps input value to another value range
    // TODO: Use C++ templates for different data types
    static float map(float x, float in_min, float in_max, float out_min, float out_max, bool clamp)
    {
        float ret = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        
        if (!clamp){
            return ret;
        }
        
        if (ret < out_min)
            return out_min;
        else if (ret > out_max)
            return out_max;
        else
            return ret;
    }
    
    static std::string btos(bool x)
    {
        if (x)
            return "True";
        else
            return "False";
    }
    
    static std::string ftos(float x, int precision)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << x;
        
        return stream.str();
    }
    
    static std::string paramToName(motion_param_t param)
    {
        switch (param){
            case TILT_X:
                return "Tilt X";
            case TILT_Y:
                return "Tilt Y";
            case TILT_Z:
                return "Tilt Z";
            case ACC_X:
                return "Acceleration X";
            case ACC_Y:
                return "Acceleration Y";
            case ACC_Z:
                return "Acceleration Z";
            case ACTIVITY:
                return "Activity";
        }
    }
};
