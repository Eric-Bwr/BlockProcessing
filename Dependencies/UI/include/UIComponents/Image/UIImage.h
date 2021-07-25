#pragma once

#include <Texture.h>
#include "../UIComponent.h"
#include "../../UIStructure/QuadMesh.h"
#include "../Text/UIText.h"

class UIImage : public UIComponent {
public:
    explicit UIImage(float width = DEFAULT_WIDTH, float height = DEFAULT_HEIGHT);
    UIImage(float positionX, float positionY, float width, float height);

    void setColor(const UIColor& color);
    void setTexture(Texture* texture, float textureX, float textureY, float textureWidth, float textureHeight);
    void setTexture(Texture* texture);
    void setPosition(float positionX, float positionY) override;
    void setBounds(float positionX, float positionY, float width, float height) override;
    void setSize(float width, float height);
    void setRadii(float radii, bool upperLeft = true, bool lowerLeft = true, bool upperRight = true, bool lowerRight = true);

    QuadMesh mesh;
    Texture* texture;
    UIColor color;
private:
    int mode = 0;
};