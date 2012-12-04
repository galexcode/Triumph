#version 120

attribute vec3 position;

//uniform vec4 color_in;

//varying vec4 color_out;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
    //color_out = color_in;
}