#include "PostProcessManager.h"
#include "PostProcessShader.h"

PostProcessManager::PostProcessManager(int width, int height) : width(width), height(height) {
    frameBuffer.bind();
    colorBuffer = new Texture();
    colorBuffer->setWidth(width);
    colorBuffer->setHeight(height);
    colorBuffer->load();
    colorBuffer->minLinear();
    colorBuffer->magLinear();
    colorBuffer->nearest();
    frameBuffer.attachColorBuffer(*colorBuffer, GL_COLOR_ATTACHMENT0);
    blurVColorBuffer = new Texture();
    blurVColorBuffer->bind();
    blurVColorBuffer->setWidth(width);
    blurVColorBuffer->setHeight(height);
    blurVColorBuffer->load();
    blurVColorBuffer->minLinear();
    blurVColorBuffer->magLinear();
    blurVColorBuffer->nearest();
    frameBuffer.attachColorBuffer(*blurVColorBuffer, GL_COLOR_ATTACHMENT1);
    blurHColorBuffer = new Texture();
    blurHColorBuffer->bind();
    blurHColorBuffer->setWidth(width);
    blurHColorBuffer->setHeight(height);
    blurHColorBuffer->load();
    blurHColorBuffer->minLinear();
    blurHColorBuffer->magLinear();
    blurHColorBuffer->nearest();
    frameBuffer.attachColorBuffer(*blurHColorBuffer, GL_COLOR_ATTACHMENT2);
    renderBuffer = new RenderBuffer(GL_DEPTH_COMPONENT32F, width, height);
    frameBuffer.attachRenderBuffer(*renderBuffer, GL_DEPTH_ATTACHMENT);
    shader.bind();
    shader.addVertexShader(PostShaderVertex);
    shader.addFragmentShader(PostShaderFragment);
    shader.finish();
    blurHorizontalShader.bind();
    blurHorizontalShader.addVertexShader(PostShaderBlurHVertex);
    blurHorizontalShader.addFragmentShader(PostShaderBlurFragment);
    blurHorizontalShader.finish();
    blurVerticalShader.bind();
    blurVerticalShader.addVertexShader(PostShaderBlurVVertex);
    blurVerticalShader.addFragmentShader(PostShaderBlurFragment);
    blurVerticalShader.finish();
    float vertices[8] = {-1, 1, -1, -1, 1, 1, 1, -1};
    vao.bind();
    vbo.init(vertices, 8, GL_STATIC_DRAW);
    auto layout = VertexBufferObjectLayout();
    layout.pushFloat(2);
    vao.addBuffer(vbo, layout);
    frameBuffer.unbind();
}

void PostProcessManager::bind() {
    frameBuffer.bind();
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void PostProcessManager::unbind() {
    frameBuffer.unbind();
}

void PostProcessManager::processEffects() {
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    if (hasBlur) {
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glClear(GL_COLOR_BUFFER_BIT);
        blurVerticalShader.bind();
        vao.bind();
        colorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawBuffer(GL_COLOR_ATTACHMENT2);
        glClear(GL_COLOR_BUFFER_BIT);
        blurHorizontalShader.bind();
        vao.bind();
        blurVColorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    frameBuffer.unbind();
    shader.bind();
    vao.bind();
    if (hasBlur)
        blurHColorBuffer->bind();
    else
        colorBuffer->bind();
    for (auto effect : effects) {
        if (effect->type == 1) {
            shader.setUniform1f("time", ((ScreenShakeEffect *) effect)->getTime());
            shader.setUniform1f("strength", ((ScreenShakeEffect *) effect)->getStrength());
        } else if (effect->type == 3) {
            shader.setUniform1f("contrast", ((ContrastEffect *) effect)->getContrast());
        }
    }
}

void PostProcessManager::drawEffects() {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PostProcessManager::process() {
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    if (hasBlur) {
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glClear(GL_COLOR_BUFFER_BIT);
        blurVerticalShader.bind();
        vao.bind();
        colorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawBuffer(GL_COLOR_ATTACHMENT2);
        glClear(GL_COLOR_BUFFER_BIT);
        blurHorizontalShader.bind();
        vao.bind();
        blurVColorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    frameBuffer.unbind();
    shader.bind();
    vao.bind();
    if (hasBlur)
        blurHColorBuffer->bind();
    else
        colorBuffer->bind();
    for (auto effect : effects) {
        if (effect->type == 1) {
            shader.setUniform1f("time", ((ScreenShakeEffect *) effect)->getTime());
            shader.setUniform1f("strength", ((ScreenShakeEffect *) effect)->getStrength());
        } else if (effect->type == 3) {
            shader.setUniform1f("contrast", ((ContrastEffect *) effect)->getContrast());
        }
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool PostProcessManager::hasErrors() {
    return shader.hasError() || blurHorizontalShader.hasError() || blurVerticalShader.hasError() || colorBuffer->hasError() || blurVColorBuffer->hasError() || blurHColorBuffer->hasError();
}

std::string PostProcessManager::getErrorMessage() {
    std::string result;
    if(hasErrors()){
        result.append("PostProcessManager: \n");
        if(shader.hasError())
            result.append(shader.getErrorMessage().c_str());
        if(blurHorizontalShader.hasError())
            result.append(blurHorizontalShader.getErrorMessage().c_str());
        if(blurVerticalShader.hasError())
            result.append(blurVerticalShader.getErrorMessage().c_str());
        if(colorBuffer->hasError())
            result.append(colorBuffer->getErrorMessage().c_str());
        if(blurVColorBuffer->hasError())
            result.append(blurVColorBuffer->getErrorMessage().c_str());
        if(blurHColorBuffer->hasError())
            result.append(blurHColorBuffer->getErrorMessage().c_str());
        result.append("\n");
    }
    return result;
}

void PostProcessManager::add(PostProcessEffect &effect) {
    for (auto element : effects) {
        if (element == &effect)
            return;
    }
    if (effect.type == 1) {
        shader.bind();
        shader.setUniformBool("hasShake", true);
    } else if (effect.type == 2) {
        shader.bind();
        shader.setUniformBool("hasGrayscale", true);
    } else if (effect.type == 3) {
        shader.bind();
        shader.setUniformBool("hasContrast", true);
    } else if (effect.type == 4) {
        shader.bind();
        shader.setUniformBool("hasGaussianBlur", true);
        hasBlur = true;
        blurHorizontalShader.bind();
        blurHorizontalShader.setUniform1f("targetFrameBufferWidth", width / ((GaussianBlurEffect&)effect).getDivisor());
        blurVerticalShader.bind();
        blurVerticalShader.setUniform1f("targetFrameBufferHeight", height / ((GaussianBlurEffect&)effect).getDivisor());
    }
    effects.emplace_back(&effect);
}

void PostProcessManager::remove(PostProcessEffect &effect) {
    if (effect.type == 1) {
        shader.bind();
        shader.setUniformBool("hasShake", false);
    } else if (effect.type == 2) {
        shader.bind();
        shader.setUniformBool("hasGrayscale", false);
    } else if (effect.type == 3) {
        shader.bind();
        shader.setUniformBool("hasContrast", false);
    } else if (effect.type == 4) {
        shader.bind();
        shader.setUniformBool("hasGaussianBlur", false);
        hasBlur = false;
    }
    for (int i = 0; i < effects.size(); i++) {
        if (&effect == effects.at(i))
            effects.erase(effects.begin() + i);
    }
}

void PostProcessManager::resize(int width, int height) {
    frameBuffer.resize(width, height);
    blurHorizontalShader.bind();
    blurHorizontalShader.setUniform1f("targetFrameBufferWidth", width);
    blurVerticalShader.bind();
    blurVerticalShader.setUniform1f("targetFrameBufferHeight", height);
    colorBuffer->resize(width, height);
    blurVColorBuffer->resize(width, height);
    blurHColorBuffer->resize(width, height);
}

PostProcessManager::~PostProcessManager() {
    effects.clear();
    delete colorBuffer;
    delete blurVColorBuffer;
    delete blurHColorBuffer;
    delete renderBuffer;
}