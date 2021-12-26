#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Texture.h>
#include <Math/Matrix.h>
#include "BlockProcessing/Game/Paths.h"

class Clouds {
public:
    void init();
    void update(double deltaFrameTime);
    void render(Mat4f& view, const Vec3d& playerPos);
    void setProjection(Mat4f& projection);
    ~Clouds();
private:
    float cloud = 0.0f, cloudSpeed = 0.003f;
    Shader* shader;
    Texture* texture;
};