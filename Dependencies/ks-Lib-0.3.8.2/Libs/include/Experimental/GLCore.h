#pragma once
#include <cstdint>
#include "../Graphics/Image.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <array>

/*namespace gfx {

    enum RenderType {
        textureShaderImage,
        guiShaderImage
    };

    struct RenderBuffer {
        Shader shader;
        VertexArray vao;
        std::vector<uint32_t> indices;
        gfx::IndexBuffer indexBuffer;
        uint64_t indexBufferSize = 0;

        std::vector<gfx::Vertex> vertex;
        gfx::VertexBuffer vertexBuffer;
        uint64_t vertexBufferSize = 0;
    };

    std::array<gfx::Vertex, 4> createQuad(float x, float y, float z, float width, float height, float r, float g, float b, float a, float textureID);
    std::array<gfx::Vertex, 4> createQuad2(float x, float y, float z, float width, float height, float r, float g, float b, float a, float textureID);
    void setUpGLRenderer(int windowWidth, int windowHeight, uint64_t bufferSize, bool debugMessage);
    GLuint getRectShaderID();
    GLuint getTexShaderID();
    GLuint getGuiShaderID();
    RenderBuffer* createRenderBuffer(uint64_t bufferSize, bool image,  const char* filePathVertex, const char* filePathFragment);
    void bindTextureSlot(RenderType type, Image& image);
    void draw();
    void drawTexture(float x, float y, float z, int width, int height, Image& image);
    void drawQuad(float x, float y, float z, int width, int height, float r, float g, float b, float a);
    void drawGuiTexElement(int x, int y, float z, int width, int height, Image& image);
    void drawGuiElement(int x, int y, float z, int width, int height, float r, float g, float b, float a);
}*/