#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in float inID;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out float id;

void main(){
    id = inID;
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
}

    #fragment
    #version 330 core

in float id;

out vec4 FragColor;

void main(){
    if(id == 0)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else if(id == 1)
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    else if(id == 2)
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    else
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}