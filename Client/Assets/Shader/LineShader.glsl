#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 projection;
uniform mat4 viewModel;

void main(){
    gl_Position = projection * viewModel * vec4(inPosition, 1.0f);
}

    #fragment
    #version 330 core

uniform vec3 color;

out vec4 FragColor;

void main(){
    FragColor = vec4(color, 1.0);
}