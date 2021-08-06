#vertex
#version 330 core

layout (location = 0) in vec2 inPosition;

out vec2 TexCoords;

void main(){
    TexCoords = (inPosition + 1.0) / 2.0;
    gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
}

#fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main(){
    FragColor = texture(screenTexture, TexCoords);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.7)
        FragColor = vec4(FragColor.rgb, 1.0);
    else
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}