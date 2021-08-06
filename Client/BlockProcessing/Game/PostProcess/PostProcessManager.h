#pragma once

#include <FrameBuffer.h>
#include <Texture.h>
#include <Shader.h>
#include <Buffer.h>
#include "BlockProcessing/Game/PostProcess/Effects/PostProcessEffect.h"
#include "BlockProcessing/Game/PostProcess/Effects/ScreenShakeEffect.h"
#include "BlockProcessing/Game/PostProcess/Effects/GrayscaleEffect.h"
#include "BlockProcessing/Game/PostProcess/Effects/ContrastEffect.h"
#include "BlockProcessing/Game/PostProcess/Effects/GaussianBlurEffect.h"

class PostProcessManager {
public:
    void init(int width, int height);
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