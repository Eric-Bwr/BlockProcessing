#include "Clouds.h"
#include <UITexture.h>

void Clouds::init() {
    shader = new Shader(SHADER_CLOUD);
    shader->addUniforms({"projection", "view", "data"});
    texture = new UITexture(TEXTURE_CLOUDS);
    texture->bind();
}

void Clouds::update(double deltaFrameTime) {
    cloud += cloudSpeed * deltaFrameTime;
}

void Clouds::render(Mat4f &view, const Vec3d& playerPos) {
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->bind();
    texture->bind();
    auto matrix = view;
    matrix.removeTranslation();
    shader->setUniform4f("data", playerPos.x, playerPos.y, playerPos.z, cloud);
    shader->setUniformMatrix4f("view", matrix.getBuffer());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glEnable(GL_CULL_FACE);
}

void Clouds::setProjection(Mat4f &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
    texture->bind();
    texture->mirroredRepeat();
}

Clouds::~Clouds() {
    delete shader;
    delete texture;
}
