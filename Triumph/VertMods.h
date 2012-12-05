//
//  VertMods.h
//  Triumph
//
//  Created by Justin Labak on 11/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__VertMods__
#define __Triumph__VertMods__

#include "Geometry.h"

enum VertModFunc       // various periodic functions
{
    FUNC_NONE = 0,
    FUNC_WAVES
};

struct Wave
{
    float amp;
    float freq;
    float speed;
    float phase;
    float offset;
    float exp;
    Vector3 dir;
    float decay;
};

//=============================================================================
// WaveFunc requires 4 params (amplitude, frequency, phase, offset).
// The equation is: amp * func(freq(t - phase)) + offset
struct VertMod
{
    VertModFunc func;
    
    Wave *waves;
    int nWaves;
    Wave minWave;
    Wave maxWave;
    
    VertMod(VertModFunc func);
    ~VertMod();
    
    Wave newWave();
    
    void update(float dTime);
};

#endif /* defined(__Triumph__VertMods__) */
