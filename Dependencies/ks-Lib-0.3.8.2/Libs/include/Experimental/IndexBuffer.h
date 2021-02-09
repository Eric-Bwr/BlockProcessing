#pragma once

#include "Vertex.h"
#include "GLError.h"
#include <GL/glew.h>

namespace gfx {

    struct IndexBuffer {
        IndexBuffer() = default;
        IndexBuffer(void* data, uint32_t numIndices, uint32_t elementSize) {
            create(data, numIndices, elementSize);
        }

        virtual ~IndexBuffer() {
            GLCALL(glDeleteBuffers(1, &bufferID));
        }

        void create(void* data, uint32_t numIndices, uint32_t elementSize) {
            GLCALL(glGenBuffers(1, &bufferID));
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
            if(data == nullptr) {
                GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_DYNAMIC_DRAW));
            } else{
                GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW));
            }

        }

        void deleteBuffer() {
            GLCALL(glDeleteBuffers(1, &bufferID));
        }

        void addSubData(void* data, uint32_t numIndices, uint32_t elementSize) {
            GLCALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndices * elementSize, data));
        }

        void bind() {
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
        }

        void unbind() {
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }

    private:
        GLuint bufferID;
    };
}