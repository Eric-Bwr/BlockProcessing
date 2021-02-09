#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

out vec2 textureCoords;

void main(){
    gl_Position = proj * view * model * vec4(inPosition, 1.0f);;
    textureCoords = inTexture;
}

#fragment
#version 330 core

in vec2 textureCoords;

uniform sampler2D materialTextureDiffuse;
uniform sampler2D materialTextureNormal;

out vec4 outColor;

void main(){
    vec4 texColor = texture(materialTextureDiffuse, textureCoords);
    if(texColor.a < 0.9)
        discard;
    outColor = texColor;
}