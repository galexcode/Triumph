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
    w.speed = RandomFloat(minWave.freq, maxWave.freq);
    w.phase = w.speed * w.freq;
    w.dir = Vector3(rand() % 1000 / 1000.0f, 0, rand() % 1000 / 1000.0f);
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
            minWave.amp = 2.0f; // 2
            minWave.freq = 0.3f; // .3
            minWave.speed = 50.0f; // 50
            minWave.exp = 2;
            minWave.decay = 0.001f;
            
            maxWave.amp = 3.0f; // 3
            maxWave.freq = 0.5f; // .5
            maxWave.speed = 150.0f; // 150
            maxWave.exp = 2;
            maxWave.decay = 0.009f;
            
            nWaves = 3;
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
        Wave w = waves[i];
        
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

float VertMod::mod(float time, float x, float y, float z) {
    float output = 0;
    
    switch(func)
    {
        case FUNC_WAVES:
            
            for (int i = 0; i < nWaves; ++i)
                output += 2 * waves[i].amp * powf(sinf(waves[i].dir.dot(Vector3(x, y, z)) * waves[i].freq + time * waves[i].phase) / 2, waves[i].exp);
            
            break;
            
        default:
            output = 1; // no function defined
            break;
    }
    
    return output;
}