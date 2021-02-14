#vertex
#version 430

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 textureCoords;
out vec3 normals;
out vec3 fragPosition;
out float visibility;

uniform float intensity = 0.0003;
uniform float gradient = 1.2;

void main(){
    vec4 pos = model * vec4(inPosition, 1.0f);
    normals = (model * vec4(inNormal, 0.0)).xyz;
    fragPosition = inPosition;
    vec4 worldToCamera = view * pos;
    gl_Position = projection * worldToCamera;
    textureCoords = inTexture;
    visibility = clamp(exp(-pow((length(worldToCamera.xyz) * intensity), gradient)), 0.0, 1.0);
}

    #fragment
    #version 430

in vec2 textureCoords;
in vec3 normals;
in vec3 fragPosition;
in float visibility;

uniform sampler2D image;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;
uniform vec3 skyColor = vec3(0.8, 0.9, 0.9);

out vec4 FragColor;

void main()
{
    vec3 color = texture(image, textureCoords).rgb;
    // ambient
    vec3 ambient = 0.0001 * color;
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
    vec3 specular = vec3(0.1) * spec;// assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    FragColor = mix(vec4(skyColor, 1.0), FragColor, visibility);
}