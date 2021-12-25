#vertex
#version 430 core

layout (location = 0) in vec3 inBlockPosition;
layout (location = 1) in vec3 inBlockData;

uniform mat4 projection;
uniform mat4 viewModel;

out vec3 textureCoords;
flat out vec3 normals;
out vec3 fragPosition;
flat out float blockID;
out float visibility;

uniform float intensity = 0.0015;
uniform float gradient = 1.2;

const vec3 vertexData[6][5] = {
    {
    vec3(0, 0, 1),
    vec3(1, 0, 1),
    vec3(0, 1, 1),
    vec3(1, 1, 1),
    vec3(0, 1, 1)
    },
    {
    vec3(1, 0, 0),
    vec3(0, 0, 0),
    vec3(1, 1, 0),
    vec3(0, 1, 0),
    vec3(1, 1, 0)
    },
    {
    vec3(0, 0, 0),
    vec3(0, 0, 1),
    vec3(0, 1, 0),
    vec3(0, 1, 1),
    vec3(0, 1, 0)
    },
    {
    vec3(1, 0, 1),
    vec3(1, 0, 0),
    vec3(1, 1, 1),
    vec3(1, 1, 0),
    vec3(1, 1, 1)
    },
    {
    vec3(0, 1, 1),
    vec3(1, 1, 1),
    vec3(0, 1, 0),
    vec3(1, 1, 0),
    vec3(0, 1, 0)
    },
    {
    vec3(0, 0, 0),
    vec3(1, 0, 0),
    vec3(0, 0, 1),
    vec3(1, 0, 1),
    vec3(0, 0, 1)
    }
};

const vec3 normalData[6] = {
    vec3(0, 0, -1),
    vec3(0, 0, 1),
    vec3(-1, 0, 0),
    vec3(1, 0, 0),
    vec3(0, 1, 0),
    vec3(0, -1, 0)
};

const vec2 textureData[5] = {
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1),
    vec2(0, 1)
};

layout(std430, binding = 0) buffer datab {
    float blockData[];
} data;

void main(){
    int face = int(inBlockData.z);
    face = int(data.blockData[gl_InstanceID * 6 + 5]);
    vec3 position = vertexData[face][gl_VertexID];
    vec3 chunkPos = vec3(data.blockData[gl_InstanceID * 6 + 0], data.blockData[gl_InstanceID * 6 + 1], data.blockData[gl_InstanceID * 6 + 2]);
    vec4 pos = viewModel * vec4(position + chunkPos, 1.0f);
    normals = normalData[face];
    fragPosition = position;
    gl_Position = projection * pos;
    textureCoords = vec3(textureData[gl_VertexID], data.blockData[gl_InstanceID * 6 + 4]);
    visibility = clamp(exp(-pow((length(pos.xyz) * intensity), gradient)), 0.0, 1.0);
    blockID = data.blockData[gl_InstanceID * 6 + 3];
}

#fragment
#version 430 core

in vec3 textureCoords;
flat in vec3 normals;
in vec3 fragPosition;
flat in float blockID;
in float visibility;

uniform sampler2DArray image;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn = true;
uniform vec3 skyColor = vec3(0.8, 0.9, 0.9);

const vec3 grassColor = vec3(0.2, 0.45, 0.2);

out vec4 FragColor;

void main(){
    vec3 color = texture(image, textureCoords).rgb;
    if (blockID == 4 && normals.y == 1)
        color *= grassColor;
    // ambient
    vec3 ambient = 0.1005 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPosition);
    vec3 normal = normalize(normals);
    float diff = max(dot(lightDir, normal), 0.2);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if (blinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = vec3(0.175) * spec;// assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    FragColor = mix(vec4(skyColor, 1.0), FragColor, visibility);



   // vec2 texCoord = gl_Position.xy * 0.5 + 0.5;


    //FragColor = vec4((normals.x + 1) / 2, (normals.y + 1) / 2, (normals.z + 1 / 2), 1);
   // FragColor = vec4(color, 1);
}