//
//  VertMods.cpp
//  Triumph
//
//  Created by Justin Labak on 11/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "VertMods.h"
#include "Geometry.h"
#include <math.h>
#include <stdlib.h>

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

Wave VertMod::newWave() {
    Wave w;
    
    w.amp = RandomFloat(minWave.amp, maxWave.amp);
    w.freq = RandomFloat(minWave.freq, maxWave.freq);
    w.speed = RandomFloat(minWave.speed, maxWave.speed);
    w.phase = w.speed * w.freq;
    w.dir = Vector3(rand() % 1000 - 500, 0, rand() % 1000 - 500);
    w.dir.normalize();
    w.exp = (int)RandomFloat(minWave.exp, maxWave.exp);
    w.decay = RandomFloat(minWave.decay, maxWave.decay);
    
    return w;
}

VertMod::VertMod(VertModFunc f) {
    func = f;
    
    switch (func)
    {
        case FUNC_WAVES:
        {
            minWave.amp = 1.0f; // 2
            minWave.freq = 0.05f; // .3
            minWave.speed = 3.0f; // 50
            minWave.exp = 2;
            minWave.decay = 0.001f;
            
            maxWave.amp = 1.5f; // 3
            maxWave.freq = 0.1f; // .5
            maxWave.speed = 30.0f; // 150
            maxWave.exp = 4;
            maxWave.decay = 0.009f;
            
            nWaves = 15;
            waves = new Wave[nWaves];
            
            for (int i = 0; i < nWaves; ++i) {
                waves[i] = newWave();
            }
            
        }
        break;
        default:
            nWaves = 0;
            break;
    }
}

VertMod::~VertMod() {
    delete [] waves;
}

void VertMod::update(float dTime) {
    for (int i = 0; i < nWaves; ++i) {
        waves[i].amp -= waves[i].decay;
        if (waves[i].amp <= 0) {
            waves[i] = newWave();
            waves[i].amp = 0;
            waves[i].decay *= -1;
        }
        else if (waves[i].amp >= maxWave.amp)
            waves[i].decay *= -1;
    }
}