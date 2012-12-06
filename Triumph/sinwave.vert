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
    vec2 dir;
};
uniform Wave Waves[15];

varying vec3 N;
varying vec3 V;
varying vec3 L;

void main()
{
    // first modulate vertices based on a wave function
    vec2 xz = AttrPosition.xz;
    vec3 newPos = AttrPosition;
    vec3 norm = vec3(0, 0, 0);
    for (int i = 0; i < NumWaves; i++) {
        Wave w = Waves[i];
        float frac = (sin(dot(w.dir, xz) * w.freq + Time * w.phase) + 1) / 2;

        // update the height position
        float y = 2 * w.amp * pow(frac, w.exp);
        
        float normFactor = w.exp * w.freq * w.amp * pow(frac, w.exp - 1) * cos(dot(w.dir, xz) * w.freq + Time * w.phase);

        // update the normal
        norm = norm + normFactor * vec3(w.dir.x, 0, w.dir.y);
        newPos = newPos + vec3(0, y, 0);
    }
    norm = norm * -1;
    norm.y = 1;
    normalize(norm);

    // next apply a blinn lighting shader
    N = gl_NormalMatrix * norm;
    V = -vec3(gl_ModelViewMatrix * vec4(newPos, 1.0));
	L = vec3(gl_ModelViewMatrix * (vec4(LightPosition,1) - vec4(newPos, 1.0)));

    gl_Position = gl_ModelViewProjectionMatrix * vec4(newPos, 1.0);
}