#vertex
#version 400 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inRounding;
layout (location = 3) in vec4 inInfo;

out vec4 info;
out vec2 textureCoords;
out vec2 roundingCoords;
out float corners;

uniform mat4 ortho;

void main() {
    gl_Position = ortho * vec4(inPosition, 0.0, 1.0);
    textureCoords = inTexture;
    roundingCoords = inRounding.xy * inInfo.zw;
    corners = inRounding.z;
    info = inInfo;
}

#fragment
#version 400 core

in vec4 info;
in vec2 textureCoords;
in vec2 roundingCoords;
in float corners;

uniform float smoothness;
uniform vec4 color;
uniform sampler2D image;

out vec4 FragColor;

float roundCorners(float alpha){
    vec4 radii = vec4(int(corners) >> 3 & 1, int(corners) >> 2 & 1, int(corners) & 1, int(corners) >> 1& 1) * info.y;
    float yMaxY = info.w - radii.y;
    float xMaxZ = info.z - radii.z;
    float yMaxZ = info.w - radii.z;
    float xMaxW = info.z - radii.w;
    if (roundingCoords.x < radii.x && roundingCoords.y < radii.x)
        alpha *= 1.0 - smoothstep(radii.x - smoothness, radii.x + smoothness, length(roundingCoords - vec2(radii.x)));
    else if (roundingCoords.x < radii.y && roundingCoords.y > yMaxY)
        alpha *= 1.0 - smoothstep(radii.y - smoothness, radii.y + smoothness, length(roundingCoords - vec2(radii.y, yMaxY)));
    else if (roundingCoords.x > xMaxZ && roundingCoords.y > yMaxZ)
        alpha *= 1.0 - smoothstep(radii.z - smoothness, radii.z + smoothness, length(roundingCoords - vec2(xMaxZ, yMaxZ)));
    else if (roundingCoords.x > xMaxW && roundingCoords.y < radii.w)
        alpha *= 1.0 - smoothstep(radii.w - smoothness, radii.w + smoothness, length(roundingCoords - vec2(xMaxW, radii.w)));
    return alpha;
}

void main() {
    if (info.x == 0 || info.x == 1)
        FragColor = color;
    else if (info.x == 2)
        FragColor = texture(image, textureCoords);
    else if (info.x == 3)
        FragColor = texture(image, textureCoords) * color;
    FragColor = vec4(FragColor.rgb, roundCorners(FragColor.a));
}