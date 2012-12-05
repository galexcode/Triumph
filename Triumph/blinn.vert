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
    vec2 xz = AttrPosition.xz;
    vec3 newPos = AttrPosition;
    vec3 norm = vec3(0, 0, 0);
    for (int i = 0; i < NumWaves; i++) {
        float frac = (sin(dot(Waves[i].dir, xz) * Waves[i].freq + Time * Waves[i].phase) + 1) / 2;

        // update the height position
        float y = 2 * Waves[i].amp * pow(frac, Waves[i].exp);
        
        
        float normFactor = Waves[i].exp * Waves[i].freq * Waves[i].amp * pow(frac, Waves[i].exp - 1) * cos(dot(Waves[i].dir, xz) * Waves[i].freq + Time * Waves[i].phase);

        // update the normal
        norm = norm + normFactor * vec3(Waves[i].dir.x, 0, Waves[i].dir.y);
        newPos = newPos + vec3(0, y, 0);
        
        //float angle = (pos.x + Waves[i].phase * Time) * Waves[i].freq;
        //newPos.y = newPos.y + sin(angle) * Waves[i].amp;
        //norm = normalize(vec3(-Waves[i].amp * Waves[i].freq * cos(angle), 1.0, 0));
    }
    norm = norm * -1;
    norm.y = 1;
    normalize(norm);
    //norm = AttrNormal;

    // for blinn lighting
    N = gl_NormalMatrix * norm;
    V = -vec3(gl_ModelViewMatrix * vec4(newPos, 1.0));
	L = vec3(gl_ModelViewMatrix * (vec4(LightPosition,1) - vec4(newPos, 1.0)));

    gl_Position = gl_ModelViewProjectionMatrix * vec4(newPos, 1.0);
}