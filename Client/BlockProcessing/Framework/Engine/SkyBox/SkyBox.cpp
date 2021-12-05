#include "SkyBox.h"

void SkyBox::init(const char *path, const char* ending) {
    std::string appendPath = path;
    auto side0 = UITexture((appendPath + char('0') + "." + ending).data());
    uint8_t* data[6] = {};
    data[0] = side0.getData();
    for(int i = 1; i < 6; i++){
        auto side = new UITexture((appendPath + char('0' + i) + "." + ending).data());
        data[i] = side->getData();
    }
    texture = new Texture(GL_TEXTURE_CUBE_MAP);
    texture->bind();
    texture->setWidth(side0.getWidth());
    texture->setHeight(side0.getHeight());
    texture->setFormat(side0.getFormat());
    texture->setInternalFormat(side0.getInternalFormat());
    texture->minLinear();
    texture->magLinear();
    texture->clampEdge();
    for(int i = 0; i < 6; i++){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, side0.getInternalFormat(), side0.getWidth(), side0.getHeight(), 0, side0.getFormat(), GL_UNSIGNED_BYTE, data[i]);
    }
}

void SkyBox::update(float skyBoxRotation, float rotationX, float rotationY, float rotationZ){
    this->skyBoxRotation = skyBoxRotation;
    this->rotationX = rotationX;
    this->rotationY = rotationY;
    this->rotationZ = rotationZ;
}

SkyBox::~SkyBox() {
    delete texture;
}