#version 120

varying vec4 interpColor;

void main()
{
    gl_FragColor = interpColor;
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}