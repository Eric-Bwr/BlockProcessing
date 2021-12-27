#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 projection;
uniform mat4 view;
uniform vec2 parameters;

layout(std430, binding = 0) buffer data {
    float octreeData[];
};

out vec3 color;

void main(){
    int level = int(octreeData[gl_InstanceID * 4 + 3]);
    if(level == 0)
        color = vec3(1.0, 0.0, 0.0);
    else if(level == parameters.x)
        color = vec3(0.0, 0.0, 0.0);
    vec3 offset = vec3(octreeData[gl_InstanceID * 4 + 0], octreeData[gl_InstanceID * 4 + 1], octreeData[gl_InstanceID * 4 + 2]);
    gl_Position = projection * view * vec4((inPosition * (level == parameters.x ? parameters.y : 1)) + offset, 1.0f);
}

#fragment
#version 330 core

in vec3 color;

out vec4 FragColor;

void main(){
    FragColor = vec4(color, 1.0);
}