#include "UITexture.h"
#include "UIStructure/STB.h"

UITexture::UITexture(const char *path, int desiredChannels) : Texture() {
    setPath(path);
    int width, height, nrComponents;
    uint8_t *data = stbi_load(path, &width, &height, &nrComponents, desiredChannels);
    if(desiredChannels != 0)
        nrComponents = desiredChannels;
    if (nrComponents == 1)
        setFormat(GL_RED);
    else if (nrComponents == 3)
        setFormat(GL_RGB);
    else if (nrComponents == 4)
        setFormat(GL_RGBA);
    setWidth(width);
    setHeight(height);
    setData(data);
    load();
    clampEdge();
    minLinear();
    magLinear();
}