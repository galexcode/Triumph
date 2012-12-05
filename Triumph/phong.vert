#version 120

attribute vec3 position_in;
attribute vec3 normal_in;
 
varying vec4 vpeye;
varying vec4 vneye;
 
void main() {
	vpeye = gl_ModelViewMatrix * vec4(position_in, 1.0);
	vneye = vec4(gl_NormalMatrix * normal_in, 0);
	gl_Position = gl_ModelViewProjectionMatrix * vec4(position_in, 1.0);
}