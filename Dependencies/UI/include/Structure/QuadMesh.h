#pragma once

#include "DataManager.h"
#include "UIColor.h"

struct UIColorTriplet {
    UIColorTriplet& operator=(const UIColor& color);
    UIColor standard, hover, pressed;
};

class QuadMesh {
public:
    QuadMesh();
    void loadPosition(float positionX, float positionY, float width, float height);
    void loadPosition(float positionX, float positionY, float width, float height, float info);
    void load(float positionX, float positionY, float width, float height, float info);
    void load(float positionX, float positionY, float width, float height, float textureWidth, float textureHeight, float textureX, float textureY, float textureRegionWidth, float textureRegionHeight, float info);
    void render();
    ~QuadMesh();
    VertexArrayObject vao;
    VertexBufferObject vbo;
    void setRadii(float radii, bool upperLeft, bool lowerLeft, bool upperRight, bool lowerRight);
    float radii = 0, corners = 0 << 8 | 0 << 6 | 0 << 4 | 0 << 2;
private:
    std::vector<float> vertices;
};

class QuadMeshTriplet {
public:
    void loadPosition(float x, float y, float w, float h,
                      float xx, float yy, float ww, float hh,
                      float xxx, float yyy, float www, float hhh);
    void loadPosition(float x, float y, float w, float h,
                      float xx, float yy, float ww, float hh,
                      float xxx, float yyy, float www, float hhh,
                      float info);
    void loadPosition(float x, float y, float w, float h);
    void loadPosition(float x, float y, float w, float h, float info);
    void load(float x, float y, float w, float h,
              float xx, float yy, float ww, float hh,
              float xxx, float yyy, float www, float hhh, float info);
    void load(float x, float y, float w, float h, float info);
    void load(float x, float y, float w, float h,
              float xx, float yy, float ww, float hh,
              float xxx, float yyy, float www, float hhh,
              float info, float textureWidth, float textureHeight,
              float texX, float texY, float texW, float texH,
              float texXX, float texYY, float texWW, float texHH,
              float texXXX, float texYYY, float texWWW, float texHHH);
    void load(float x, float y, float w, float h,
              float info, float textureWidth, float textureHeight,
              float texX, float texY, float texW, float texH,
              float texXX, float texYY, float texWW, float texHH,
              float texXXX, float texYYY, float texWWW, float texHHH);
    void render(int index);
    void setRadii(float radii, bool upperLeft, bool lowerLeft, bool upperRight, bool lowerRight);
    float radii = 0;
private:
    QuadMesh meshes[3];
};