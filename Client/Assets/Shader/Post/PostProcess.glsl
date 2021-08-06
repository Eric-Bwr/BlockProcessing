#vertex
#version 330 core

layout (location = 0) in vec2 inPosition;

uniform float time;
uniform float strength;
uniform bool hasShake = false;

out vec2 textureCoords;

void main(){
    textureCoords = (inPosition + 1.0) / 2.0;
    gl_Position = vec4(inPosition, 0.0, 1.0);
    if(hasShake){
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}

#fragment
#version 330 core

in vec2 textureCoords;

uniform sampler2D screenTexture;
uniform bool hasGrayscale = false;
uniform bool hasContrast = false;
uniform float contrast;

out vec4 fragColor;

void main(){
    vec4 fragment = texture(screenTexture, textureCoords);
    if(hasGrayscale){
        fragment = texture(screenTexture, textureCoords);
        float average = 0.2126 * fragment.r + 0.7152 * fragment.g + 0.0722 * fragment.b;
        fragment = vec4(average, average, average, 1.0);
    }
    if(hasContrast){
        fragment.rgb = (fragment.rgb - 0.5) * (1.0 + contrast) + 0.5;
    }
    fragColor = fragment;
}