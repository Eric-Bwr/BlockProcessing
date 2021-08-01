#vertex
#version 430 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in float inBlockID;

uniform mat4 projection;
uniform mat4 modelView;

out vec2 textureCoords;
out vec3 normals;
out vec3 fragPosition;
flat out float blockID;
out float visibility;

uniform float intensity = 0.00015;
uniform float gradient = 1.2;

void main(){
    vec4 pos = modelView * vec4(inPosition, 1.0f);
    normals = inNormal;
    fragPosition = inPosition;
    gl_Position = projection * pos;
    textureCoords = inTexture;
    visibility = clamp(exp(-pow((length(pos.xyz) * intensity), gradient)), 0.0, 1.0);
    blockID = inBlockID;
}

#fragment
#version 430 core

in vec2 textureCoords;
in vec3 normals;
in vec3 fragPosition;
flat in float blockID;
in float visibility;

uniform sampler2D image;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn = true;
uniform vec3 skyColor = vec3(0.8, 0.9, 0.9);

const vec3 grassColor = vec3(0.2, 0.45, 0.2);

out vec4 FragColor;

void main(){
    vec3 color = texture(image, textureCoords).rgb;
    if (blockID == 3.0)
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
}