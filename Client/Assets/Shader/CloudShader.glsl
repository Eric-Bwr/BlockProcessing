#vertex
#version 330 core

uniform mat4 projection;
uniform mat4 view;
uniform vec4 data;

const int cloudSize = 1500;
const float cloudHeight = 300.15;

const vec3 vertexData[5] = {
    vec3(-1 * cloudSize, cloudHeight, 1 * cloudSize),
    vec3(1 * cloudSize, cloudHeight, 1 * cloudSize),
    vec3(-1 * cloudSize, cloudHeight, -1 * cloudSize),
    vec3(1 * cloudSize, cloudHeight, -1 * cloudSize),
    vec3(-1 * cloudSize, cloudHeight, -1 * cloudSize)
};

const vec2 textureData[5] = {
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1),
    vec2(0, 1)
};

out vec2 textureCoords;

void main(){
    vec3 pos = vertexData[gl_VertexID];
    pos.y -= data.y;
    gl_Position = projection * view * vec4(pos, 1.0f);
    textureCoords = textureData[gl_VertexID];
    textureCoords.x += data.x / cloudSize / 2;
    textureCoords.y -= data.z / cloudSize / 2;
    textureCoords.y += data.w;
}

#fragment
#version 330 core

in vec2 textureCoords;

uniform sampler2D image;

out vec4 FragColor;

void main(){
    vec4 color = texture(image, textureCoords);
    if(color.a < 0.1)
        discard;
    color.a = 0.5;
    FragColor = color;
}