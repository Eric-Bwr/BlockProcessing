#pragma once

#include <cstdint>
#include <GL/glew.h>

namespace gfx {
    struct VertexArray {
        VertexArray() = default;

        virtual ~VertexArray() {
            GLCALL(glDeleteBuffers(1, &bufferID));
        }

        void create() {
            GLCALL(glGenVertexArrays(1, &bufferID));
            GLCALL(glBindVertexArray(bufferID));
        }

        void deleteBuffer() {
            GLCALL(glDeleteVertexArrays(1, &bufferID));
        }

        void bind() {
            GLCALL(glBindVertexArray(bufferID));
        }

        void unbind() {
            GLCALL(glBindVertexArray(0));
        }

    private:
        GLuint bufferID;
    };
}