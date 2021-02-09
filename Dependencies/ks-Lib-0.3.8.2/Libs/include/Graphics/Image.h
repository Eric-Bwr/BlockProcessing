// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include <GL/glew.h>

namespace gfx {
    class Image {
    private:
        int width;
        int height;
        int channels;
        GLuint bufferID;
        const char* imageName = nullptr;
        GLuint texIndex = 0;
    public:
        Image();
        Image(int width, int height, int channels, GLuint bufferID, const char* name);
        ~Image() = default;
        int getWidth();
        int getHeight();
        int getChannels();
        GLuint getBufferID();
        GLuint getTextureIndex();

        void bind();
        void unbind();
        void deleteSource();
    };
}
