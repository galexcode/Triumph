#version 120

uniform sampler1D GaussianTexture;

void main()
{
    
    vec3 halfAngle = normalize(lightDir + viewDirection);
    float texCoord = dot(halfAngle, surfaceNormal);
    float gaussianTerm = texture(GaussianTexture, texCoord).r;
    
    gaussianTerm = cosAngIncidence != 0.0 ? gaussianTerm : 0.0;
    
}