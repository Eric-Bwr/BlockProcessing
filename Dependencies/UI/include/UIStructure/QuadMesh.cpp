#include "QuadMesh.h"

UIColorTriplet &UIColorTriplet::operator=(const UIColor &color) {
    standard = color, hover = color, pressed = color;
    return *this;
}

QuadMesh::QuadMesh() {
    vao.init();
    vbo.init(nullptr, 0, GL_STATIC_DRAW);
    vao.addBuffer(vbo, DataManager::quadLayout);
    vertices.resize(66);
}

void QuadMesh::loadPosition(float positionX, float positionY, float width, float height) {
    vertices.at(0) = positionX;
    vertices.at(1) = positionY + height;
    vertices.at(6) = corners;
    vertices.at(8) = radii;
    vertices.at(9) = width;
    vertices.at(10) = height;

    vertices.at(11) = positionX;
    vertices.at(12) = positionY;
    vertices.at(17) = corners;
    vertices.at(19) = radii;
    vertices.at(20) = width;
    vertices.at(21) = height;

    vertices.at(22) = positionX + width;
    vertices.at(23) = positionY;
    vertices.at(28) = corners;
    vertices.at(30) = radii;
    vertices.at(31) = width;
    vertices.at(32) = height;

    vertices.at(33) = positionX;
    vertices.at(34) = positionY + height;
    vertices.at(39) = corners;
    vertices.at(41) = radii;
    vertices.at(42) = width;
    vertices.at(43) = height;

    vertices.at(44) = positionX + width;
    vertices.at(45) = positionY;
    vertices.at(50) = corners;
    vertices.at(52) = radii;
    vertices.at(53) = width;
    vertices.at(54) = height;

    vertices.at(55) = positionX + width;
    vertices.at(56) = positionY + height;
    vertices.at(61) = corners;
    vertices.at(63) = radii;
    vertices.at(64) = width;
    vertices.at(65) = height;

    vbo.subData(vertices.data(), 6 * DataManager::quadLayout.getStride(), 0, GL_STATIC_DRAW);
}

void QuadMesh::loadPosition(float positionX, float positionY, float width, float height, float info) {
    vertices.at(0) = positionX;
    vertices.at(1) = positionY + height;
    vertices.at(6) = corners;
    vertices.at(7) = info;
    vertices.at(8) = radii;
    vertices.at(9) = width;
    vertices.at(10) = height;

    vertices.at(11) = positionX;
    vertices.at(12) = positionY;
    vertices.at(17) = corners;
    vertices.at(18) = info;
    vertices.at(19) = radii;
    vertices.at(20) = width;
    vertices.at(21) = height;

    vertices.at(22) = positionX + width;
    vertices.at(23) = positionY;
    vertices.at(28) = corners;
    vertices.at(29) = info;
    vertices.at(30) = radii;
    vertices.at(31) = width;
    vertices.at(32) = height;

    vertices.at(33) = positionX;
    vertices.at(34) = positionY + height;
    vertices.at(39) = corners;
    vertices.at(40) = info;
    vertices.at(41) = radii;
    vertices.at(42) = width;
    vertices.at(43) = height;

    vertices.at(44) = positionX + width;
    vertices.at(45) = positionY;
    vertices.at(50) = corners;
    vertices.at(51) = info;
    vertices.at(52) = radii;
    vertices.at(53) = width;
    vertices.at(54) = height;

    vertices.at(55) = positionX + width;
    vertices.at(56) = positionY + height;
    vertices.at(61) = corners;
    vertices.at(62) = info;
    vertices.at(63) = radii;
    vertices.at(64) = width;
    vertices.at(65) = height;

    vbo.subData(vertices.data(), 6 * DataManager::quadLayout.getStride(), 0, GL_STATIC_DRAW);
}

void QuadMesh::load(float positionX, float positionY, float width, float height, float info) {
    vertices.at(0) = positionX;
    vertices.at(1) = positionY + height;
    vertices.at(2) = 0;
    vertices.at(3) = 1;
    vertices.at(4) = 0;
    vertices.at(5) = 1;
    vertices.at(6) = corners;
    vertices.at(7) = info;
    vertices.at(8) = radii;
    vertices.at(9) = width;
    vertices.at(10) = height;

    vertices.at(11) = positionX;
    vertices.at(12) = positionY;
    vertices.at(13) = 0;
    vertices.at(14) = 0;
    vertices.at(15) = 0;
    vertices.at(16) = 0;
    vertices.at(17) = corners;
    vertices.at(18) = info;
    vertices.at(19) = radii;
    vertices.at(20) = width;
    vertices.at(21) = height;

    vertices.at(22) = positionX + width;
    vertices.at(23) = positionY;
    vertices.at(24) = 1;
    vertices.at(25) = 0;
    vertices.at(26) = 1;
    vertices.at(27) = 0;
    vertices.at(28) = corners;
    vertices.at(29) = info;
    vertices.at(30) = radii;
    vertices.at(31) = width;
    vertices.at(32) = height;

    vertices.at(33) = positionX;
    vertices.at(34) = positionY + height;
    vertices.at(35) = 0;
    vertices.at(36) = 1;
    vertices.at(37) = 0;
    vertices.at(38) = 1;
    vertices.at(39) = corners;
    vertices.at(40) = info;
    vertices.at(41) = radii;
    vertices.at(42) = width;
    vertices.at(43) = height;

    vertices.at(44) = positionX + width;
    vertices.at(45) = positionY;
    vertices.at(46) = 1;
    vertices.at(47) = 0;
    vertices.at(48) = 1;
    vertices.at(49) = 0;
    vertices.at(50) = corners;
    vertices.at(51) = info;
    vertices.at(52) = radii;
    vertices.at(53) = width;
    vertices.at(54) = height;

    vertices.at(55) = positionX + width;
    vertices.at(56) = positionY + height;
    vertices.at(57) = 1;
    vertices.at(58) = 1;
    vertices.at(59) = 1;
    vertices.at(60) = 1;
    vertices.at(61) = corners;
    vertices.at(62) = info;
    vertices.at(63) = radii;
    vertices.at(64) = width;
    vertices.at(65) = height;

    vbo.subData(vertices.data(), 6 * DataManager::quadLayout.getStride(), 0, GL_STATIC_DRAW);
}

void QuadMesh::load(float positionX, float positionY, float width, float height, float textureWidth, float textureHeight, float textureX, float textureY, float textureRegionWidth, float textureRegionHeight, float info) {
    vertices.at(0) = positionX;
    vertices.at(1) = positionY + height;
    vertices.at(2) = textureX / textureWidth;
    vertices.at(3) = (textureY + textureRegionHeight) / textureHeight;
    vertices.at(4) = 0;
    vertices.at(5) = 1;
    vertices.at(6) = corners;
    vertices.at(7) = info;
    vertices.at(8) = radii;
    vertices.at(9) = width;
    vertices.at(10) = height;

    vertices.at(11) = positionX;
    vertices.at(12) = positionY;
    vertices.at(13) = textureX / textureWidth;
    vertices.at(14) = textureY / textureHeight;
    vertices.at(15) = 0;
    vertices.at(16) = 0;
    vertices.at(17) = corners;
    vertices.at(18) = info;
    vertices.at(19) = radii;
    vertices.at(20) = width;
    vertices.at(21) = height;

    vertices.at(22) = positionX + width;
    vertices.at(23) = positionY;
    vertices.at(24) = (textureX + textureRegionWidth) / textureWidth;
    vertices.at(25) = textureY / textureHeight;
    vertices.at(26) = 1;
    vertices.at(27) = 0;
    vertices.at(28) = corners;
    vertices.at(29) = info;
    vertices.at(30) = radii;
    vertices.at(31) = width;
    vertices.at(32) = height;

    vertices.at(33) = positionX;
    vertices.at(34) = positionY + height;
    vertices.at(35) = textureX / textureWidth;
    vertices.at(36) = (textureY + textureRegionHeight) / textureHeight;
    vertices.at(37) = 0;
    vertices.at(38) = 1;
    vertices.at(39) = corners;
    vertices.at(40) = info;
    vertices.at(41) = radii;
    vertices.at(42) = width;
    vertices.at(43) = height;

    vertices.at(44) = positionX + width;
    vertices.at(45) = positionY;
    vertices.at(46) = (textureX + textureRegionWidth) / textureWidth;
    vertices.at(47) = textureY / textureHeight;
    vertices.at(48) = 1;
    vertices.at(49) = 0;
    vertices.at(50) = corners;
    vertices.at(51) = info;
    vertices.at(52) = radii;
    vertices.at(53) = width;
    vertices.at(54) = height;

    vertices.at(55) = positionX + width;
    vertices.at(56) = positionY + height;
    vertices.at(57) = (textureX + textureRegionWidth) / textureWidth;
    vertices.at(58) = (textureY + textureRegionHeight) / textureHeight;
    vertices.at(59) = 1;
    vertices.at(60) = 1;
    vertices.at(61) = corners;
    vertices.at(62) = info;
    vertices.at(63) = radii;
    vertices.at(64) = width;
    vertices.at(65) = height;

    vbo.subData(vertices.data(), 6 * DataManager::quadLayout.getStride(), 0, GL_STATIC_DRAW);
}

void QuadMesh::render() {
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void QuadMesh::setRadii(float radii, bool upperLeft, bool lowerLeft, bool upperRight, bool lowerRight) {
    this->radii = radii;
    this->corners = float((int)upperLeft << 3 | (int)lowerLeft << 2 | (int)upperRight << 1 | (int)lowerRight);
    vertices.at(6) = corners;
    vertices.at(8) = radii;
    vertices.at(17) = corners;
    vertices.at(19) = radii;
    vertices.at(28) = corners;
    vertices.at(30) = radii;;
    vertices.at(39) = corners;
    vertices.at(41) = radii;
    vertices.at(50) = corners;
    vertices.at(52) = radii;
    vertices.at(61) = corners;
    vertices.at(63) = radii;

    vbo.subData(vertices.data(), 6 * DataManager::quadLayout.getStride(), 0, GL_STATIC_DRAW);
}

QuadMesh::~QuadMesh() {
    vertices.clear();
}

void QuadMeshTriplet::loadPosition(float x, float y, float w, float h,
                                   float xx, float yy, float ww, float hh,
                                   float xxx, float yyy, float www, float hhh) {
    meshes[0].loadPosition(x, y, w, h);
    meshes[1].loadPosition(xx, yy, ww, hh);
    meshes[2].loadPosition(xxx, yyy, www, hhh);
}

void QuadMeshTriplet::loadPosition(float x, float y, float w, float h,
                                   float xx, float yy, float ww, float hh,
                                   float xxx, float yyy, float www, float hhh,
                                   float info) {
    meshes[0].loadPosition(x, y, w, h, info);
    meshes[1].loadPosition(xx, yy, ww, hh, info);
    meshes[2].loadPosition(xxx, yyy, www, hhh, info);
}

void QuadMeshTriplet::loadPosition(float x, float y, float w, float h) {
    meshes[0].loadPosition(x, y, w, h);
    meshes[1].loadPosition(x, y, w, h);
    meshes[2].loadPosition(x, y, w, h);
}

void QuadMeshTriplet::loadPosition(float x, float y, float w, float h, float info) {
    meshes[0].loadPosition(x, y, w, h, info);
    meshes[1].loadPosition(x, y, w, h, info);
    meshes[2].loadPosition(x, y, w, h, info);
}

void QuadMeshTriplet::load(float x, float y, float w, float h,
                           float xx, float yy, float ww, float hh,
                           float xxx, float yyy, float www, float hhh, float info) {
    meshes[0].load(x, y, w, h, info);
    meshes[1].load(xx, yy, ww, hh, info);
    meshes[2].load(xxx, yyy, www, hhh, info);
}

void QuadMeshTriplet::load(float x, float y, float w, float h, float info) {
    meshes[0].load(x, y, w, h, info);
    meshes[1].load(x, y, w, h, info);
    meshes[2].load(x, y, w, h, info);
}

void QuadMeshTriplet::load(float x, float y, float w, float h,
                           float xx, float yy, float ww, float hh,
                           float xxx, float yyy, float www, float hhh,
                           float info, float textureWidth, float textureHeight,
                           float texX, float texY, float texW, float texH,
                           float texXX, float texYY, float texWW, float texHH,
                           float texXXX, float texYYY, float texWWW, float texHHH) {
    meshes[0].load(x, y, w, h, textureWidth, textureHeight, texX, texY, texW, texH, info);
    meshes[1].load(xx, yy, ww, hh, textureWidth, textureHeight, texXX, texYY, texWW, texHH, info);
    meshes[2].load(xxx, yyy, www, hhh, textureWidth, textureHeight, texXXX, texYYY, texWWW, texHHH, info);
}

void QuadMeshTriplet::load(float x, float y, float w, float h,
                           float info, float textureWidth, float textureHeight,
                           float texX, float texY, float texW, float texH,
                           float texXX, float texYY, float texWW, float texHH,
                           float texXXX, float texYYY, float texWWW, float texHHH) {
    meshes[0].load(x, y, w, h, textureWidth, textureHeight, texX, texY, texW, texH, info);
    meshes[1].load(x, y, w, h, textureWidth, textureHeight, texXX, texYY, texWW, texHH, info);
    meshes[2].load(x, y, w, h, textureWidth, textureHeight, texXXX, texYYY, texWWW, texHHH, info);
}

void QuadMeshTriplet::render(int index) {
    meshes[index].vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void QuadMeshTriplet::setRadii(float radii, bool upperLeft, bool lowerLeft, bool upperRight, bool lowerRight) {
    this->radii = radii;
    meshes[0].setRadii(radii, upperLeft, lowerLeft, upperRight, lowerRight);
    meshes[1].setRadii(radii, upperLeft, lowerLeft, upperRight, lowerRight);
    meshes[2].setRadii(radii, upperLeft, lowerLeft, upperRight, lowerRight);
}
