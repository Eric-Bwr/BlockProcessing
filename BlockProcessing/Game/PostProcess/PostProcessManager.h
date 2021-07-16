#pragma once

#include <FrameBuffer.h>
#include <Texture.h>
#include <Shader.h>
#include <Buffer.h>
#include "Effects/PostProcessEffect.h"
#include "Effects/ScreenShakeEffect.h"
#include "Effects/GrayscaleEffect.h"
#include "Effects/ContrastEffect.h"
#include "Effects/GaussianBlurEffect.h"

class PostProcessManager {
public:
    PostProcessManager(int width, int height);
    void bind();
    void unbind();
    void processEffects();
    void drawEffects();
    void process();
    void add(PostProcessEffect& effect);
    void remove(PostProcessEffect& effect);
    void resize(int width, int height);
    bool hasErrors();
    std::string getErrorMessage();
    ~PostProcessManager();
private:
    std::vector<PostProcessEffect*> effects;
    FrameBuffer frameBuffer;
    RenderBuffer* renderBuffer;
    Shader shader;
    Shader blurVerticalShader;
    Shader blurHorizontalShader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    Texture* colorBuffer;
    Texture* blurVColorBuffer;
    Texture* blurHColorBuffer;
    int width, height;
    bool hasBlur = false;
};