#vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 lightPosition = vec3(40.0, 100.0, 40.0);

out vec3 lightPos;
out vec3 normals;

void main(){
    vec4 pos = model * vec4(inPosition, 1.0f);
    normals = (model * vec4(inNormal, 0.0)).xyz;
    lightPos = lightPosition - pos.xyz;
    gl_Position = projection * view * pos;
}

#fragment
#version 330 core

in vec3 lightPos;
in vec3 normals;

uniform vec4 lightColor = vec4(1.0, 1.0, 0.0, 1.0);
uniform float lightBrightness = 3.0;

out vec4 FragColor;

void main() {
    float brightness = max(dot(normalize(normals), normalize(lightPos)), 0.0);
    vec3 light = brightness * lightBrightness * lightColor.xyz;
    FragColor = vec4(light, 0.0);
}