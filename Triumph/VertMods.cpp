//
//  VertMods.cpp
//  Triumph
//
//  Created by Justin Labak on 11/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "VertMods.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define M_2PI M_PI * 2

///////////////////////////////////////////////////////////////////////////////
// compute the position at the current time(sec)
// Equation: amp * FUNC(freq*(t - phase)) + offset
///////////////////////////////////////////////////////////////////////////////
float VertMod::update(float time)
{
    // compute time factor between 0 and 1 from (freq*(time - phase))
    float timeFact = freq * (time - phase);
    timeFact -= (int)timeFact;
    
    switch(func)
    {
        case FUNC_SIN:
            output = sinf(M_2PI * timeFact);
            break;
            
        case FUNC_TRIANGLE:
            if(timeFact < 0.25f)            // 0 ~ 0.25
                output = 4 * timeFact;
            else if(timeFact < 0.75f)       // 0.25 ~ 0.75
                output = 2 - (4 * timeFact);
            else                            // 0.75 ~ 1
                output = 4 * timeFact - 4;
            break;
            
        case FUNC_SQUARE:
            if(timeFact < 0.5f)
                output = 1;
            else
                output = -1;
            break;
            
        case FUNC_SAWTOOTH:
            output = 2 * timeFact - 1;
            break;
            
        default:
            output = 1; // no function defined
    }
    
    // apply amplitude and offset
    output = amp * output + offset;
    
    return output;
}