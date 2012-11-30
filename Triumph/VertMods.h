//
//  VertMods.h
//  Triumph
//
//  Created by Justin Labak on 11/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__VertMods__
#define __Triumph__VertMods__

enum VertModFunc       // various periodic functions
{
    FUNC_NONE = 0,
    FUNC_SIN,
    FUNC_TRIANGLE,
    FUNC_SQUARE,
    FUNC_SAWTOOTH
};

//=============================================================================
// WaveFunc requires 4 params (amplitude, frequency, phase, offset).
// The equation is: amp * func(freq(t - phase)) + offset
struct VertMod
{
    VertModFunc func;
    float    amp;       // amplitude
    float    freq;      // frequency
    float    phase;     // horizontal shift
    float    offset;    // vertical shift
    float    output;    // result at given time
    
    // default constructor, initialize all members
    VertMod() : func(FUNC_NONE), amp(1.0f), freq(1.0f), phase(0.0f), offset(0.0f), output(0.0f) {}
    
    // compute the position at the current time(sec)
    float update(float time);
};

#endif /* defined(__Triumph__VertMods__) */
