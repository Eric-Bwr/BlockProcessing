#pragma once

#include <Util/Vector.h>
#include <Util/Map.h>
#include <FrameBuffer.h>
#include <Texture.h>
#include <Shader.h>
#include <ks-Lib-0.3.8.2/include/Util/String.h>
#include "Engine/2D/Model2D/Model2D.h"
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
    const char* getErrorMessage();
    ~PostProcessManager();
private:
    Vector<PostProcessEffect*> effects;
    FrameBuffer* frameBuffer;
    RenderBuffer* renderBuffer;
    Shader* shader;
    Shader* blurVerticalShader;
    Shader* blurHorizontalShader;
    Model2D* screenModel;
    Texture* colorBuffer;
    Texture* blurVColorBuffer;
    Texture* blurHColorBuffer;
    int width, height;
    bool hasBlur = false;
};