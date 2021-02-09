#vertex
#version 330 core

layout (location = 0) in vec2 inPosition;

out vec2 textureCoords[11];

uniform float targetFrameBufferHeight;

void main(void){
    gl_Position = vec4(inPosition, 0.0, 1.0);
    vec2 centerTextureCoords = inPosition * 0.5 + 0.5;
    float pixelSize = 1.0 / targetFrameBufferHeight;
    for(int i = -5; i <= 5; i++){
        textureCoords[i + 5] = centerTextureCoords + vec2(0.0, pixelSize * i);
    }
}

#fragment
#version 330 core

in vec2 textureCoords[11];

uniform sampler2D screenTexture;

out vec4 fragColor;

void main(void){
    fragColor = vec4(0.0);
    fragColor += texture(screenTexture, textureCoords[0]) * 0.0093;
    fragColor += texture(screenTexture, textureCoords[1]) * 0.028002;
    fragColor += texture(screenTexture, textureCoords[2]) * 0.065984;
    fragColor += texture(screenTexture, textureCoords[3]) * 0.121703;
    fragColor += texture(screenTexture, textureCoords[4]) * 0.175713;
    fragColor += texture(screenTexture, textureCoords[5]) * 0.198596;
    fragColor += texture(screenTexture, textureCoords[6]) * 0.175713;
    fragColor += texture(screenTexture, textureCoords[7]) * 0.121703;
    fragColor += texture(screenTexture, textureCoords[8]) * 0.065984;
    fragColor += texture(screenTexture, textureCoords[9]) * 0.028002;
    fragColor += texture(screenTexture, textureCoords[10]) * 0.0093;
}