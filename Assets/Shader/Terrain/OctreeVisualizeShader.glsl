#vertex
#version 330

layout (location = 0) in vec3 inPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
}

    #fragment
    #version 330

uniform vec3 color;

out vec4 FragColor;

void main(){
    FragColor = vec4(color, 1.0);
}