#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 proj;
uniform mat4 view;

out vec3 textureCoords;

void main() {
    textureCoords = inPosition;
    vec4 pos = proj * view * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
}

#fragment
#version 330 core
in vec3 textureCoords;

uniform samplerCube skyBox;

out vec4 outColor;

void main() {
    outColor = texture(skyBox, textureCoords);
}