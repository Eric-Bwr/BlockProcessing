#vertex
#version 330

layout (location = 0) in vec3 inPosition;

void main(){
    gl_Position = vec4(inPosition, 1.0f);
}

    #fragment
    #version 330

out vec4 FragColor;

void main(){
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}