#version 120

attribute vec3 position_in;
attribute vec3 normal_in;

uniform vec3 lightPosition;
uniform vec4 lightDiffuse;
uniform vec4 lightAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialAmbient;
uniform vec4 globalAmbient;

varying vec4 interpColor;

void main()
{
    vec3 normal, lightDir;
    vec4 diffuse, ambient, globAmbient;
    float NdotL;
    
    gl_Position = gl_ModelViewProjectionMatrix * vec4(position_in, 1.0);
    
    normal = normalize(gl_NormalMatrix * normal_in);
    lightDir = normalize(vec3(lightPosition));
    NdotL = max(dot(normal, lightDir), 0.0);
    diffuse = materialDiffuse * lightDiffuse;
    
    ambient = materialAmbient * lightAmbient;
    globAmbient = materialAmbient * globalAmbient;
    
    interpColor = NdotL * diffuse + globalAmbient + ambient;
    
}