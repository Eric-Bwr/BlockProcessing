#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 lightPosition = vec3(-10.0f, 600.0f, 0.0f);

out vec2 textureCoords;
out vec3 lightPos;
out vec3 normals;

void main(){
    vec4 pos = model * vec4(inPosition, 1.0f);
    normals = (model * vec4(inNormal, 0.0)).xyz;
    lightPos = lightPosition - pos.xyz;
    gl_Position = projection * view * pos;
    textureCoords = inTexture;
}

    #fragment
    #version 330 core

in vec2 textureCoords;
in vec3 lightPos;
in vec3 normals;

uniform sampler2D image;
uniform vec4 lightColor = vec4(0.8, 0.8, 0.8, 1.0);
uniform float lightBrightness = 1.5f;

out vec4 FragColor;

void main(){
    float brightness = max(dot(normalize(normals), normalize(lightPos)), 0.3);
    vec3 light = brightness * lightBrightness * lightColor.xyz;
    vec4 texColor = texture(image, textureCoords);
    FragColor = texColor;
}