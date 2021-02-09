#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 plane;

out vec2 textureCoords;

void main(){
    vec4 worldPosition = model * vec4(inPosition, 1.0f);
    gl_ClipDistance[0] = dot(worldPosition, plane);
    gl_Position = projection * view * worldPosition;
    textureCoords = inTexture;
}

#fragment
#version 330 core

in vec2 textureCoords;

uniform sampler2D image;

out vec4 FragColor;

void main(){
    FragColor = texture(image, textureCoords);
}