#vertex
#version 330 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in float inLevel;

uniform mat4 ortho;

out vec2 textureCoords;
out float level;

void main() {
    gl_Position = ortho * vec4(inPosition, 0.0, 1.0);
    textureCoords = inTexture;
    level = inLevel;
}

#fragment
#version 330 core

in vec2 textureCoords;
in float level;

uniform sampler2DArray image;
uniform vec4 color;

out vec4 FragColor;

void main() {
    FragColor = vec4(color.rgb, texture(image, vec3(textureCoords, level - 32)).r * color.a);
}