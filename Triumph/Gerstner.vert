#version 120

attribute vec3 AttrPosition;
attribute vec3 AttrNormal;

uniform vec3 LightPosition;
uniform float Time;

uniform int NumWaves = 0;

struct Wave
{
    float amp;
    float freq;
    float phase;
    float exp;
    float steepness;
    vec2 dir;
};
uniform Wave Waves[15];

varying vec3 N;
varying vec3 V;
varying vec3 L;

void main()
{
    // first modulate vertices based on a Gerstner wave function
    vec2 xz = AttrPosition.xz;
    vec3 P = AttrPosition;
    //P = vec3(AttrPosition.x, 0, AttrPosition.z);
    for (int i = 0; i < NumWaves; i++) {
        Wave w = Waves[i];
        
        // steepness
        float Q = w.steepness;
        
        // update the positions
        float x = Q * w.amp * w.dir.x * cos(w.freq * dot(xz, w.dir) + w.phase * Time);
        float y = w.amp * sin(w.freq * dot(xz, w.dir) + w.phase * Time);
        float z = Q * w.amp * w.dir.y * cos(w.freq * dot(xz, w.dir) + w.phase * Time);
        P = P + vec3(x, y, z);
    }
    
    // normals require final modulated positions
    vec3 norm = vec3(0, 0, 0);
    for (int i = 0; i < NumWaves; i++) {
        Wave w = Waves[i];
        float term = w.freq * dot(w.dir, P.xz) + w.phase + Time;
        float x = w.dir.x * w.freq * w.amp * cos(term);
        float y = w.steepness * w.freq * w.amp * sin(term);
        float z = w.dir.y * w.freq * w.amp * cos(term);
        norm = norm + vec3(x, y, z);
    }
    norm = norm * -1;
    norm.y = 1 + norm.y;
    
    // next apply a blinn lighting shader
    N = gl_NormalMatrix * norm;
    V = -vec3(gl_ModelViewMatrix * vec4(P, 1.0));
	L = vec3(gl_ModelViewMatrix * (vec4(LightPosition,1) - vec4(P, 1.0)));
    
    gl_Position = gl_ModelViewProjectionMatrix * vec4(P, 1.0);
}