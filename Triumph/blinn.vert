#version 120

attribute vec3 AttrPosition;
attribute vec3 AttrNormal;

uniform vec3 LightPosition;

varying vec3 N;
varying vec3 V;
varying vec3 L;

void main()
{
    // for blinn lighting
    N = gl_NormalMatrix * AttrNormal;
    V = -vec3(gl_ModelViewMatrix * vec4(AttrPosition, 1.0));
	L = vec3(gl_ModelViewMatrix * (vec4(LightPosition,1) - vec4(AttrPosition, 1.0)));

    gl_Position = gl_ModelViewProjectionMatrix * vec4(AttrPosition, 1.0);
}