#vertex

#version 330 core

layout (location = 0) in vec2 inPosition;

uniform mat4 matrix;
uniform mat4 ortho;

out vec2 textureCoords;

void main(void) {
    //gl_Position = ortho * matrix * vec4(inPosition, 0.0, 1.0);
    gl_Position = vec4(inPosition, 0.0, 1.0);
    textureCoords = (inPosition + 1.0) / 2.0;
}

    #fragment
    #version 330 core

in vec2 textureCoords;

uniform sampler2D image;
//uniform sampler2DArray image;

out vec4 outColor;

void main(void){
    outColor = texture(image, textureCoords);
 // outColor = texture(image, vec3(textureCoords, 0));
}