#version 120

uniform vec3 AmbientColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform float AmbientIntensity;
uniform float DiffuseIntensity;
uniform float SpecularIntensity;
uniform float Roughness;

varying vec3 N;
varying vec3 V;
varying vec3 L;

void main()
{ 
    vec3 l = normalize(L);
    vec3 n = normalize(N);
    vec3 v = normalize(V);
    vec3 h = normalize(l+v);

    float diffuse = dot(l,n);
    float specular = pow(max(0.0,dot(n,h)),1/Roughness);
    
    gl_FragColor = vec4(AmbientColor*AmbientIntensity +
                        DiffuseColor*diffuse*DiffuseIntensity +
                        SpecularColor*specular*SpecularIntensity,1);
}