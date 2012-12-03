#version 120

attribute vec3 position;
attribute vec3 normal;
varying vec4 color;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
    color = vec4(0.0, 1.0, 0.0, 1.0);
}