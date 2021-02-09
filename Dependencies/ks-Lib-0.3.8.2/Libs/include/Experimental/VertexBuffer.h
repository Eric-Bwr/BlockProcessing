#pragma once

#include "Vertex.h"
#include "GLError.h"
#include <GL/glew.h>

namespace gfx {

    struct VertexBuffer {
        VertexBuffer() = default;

        VertexBuffer(void* data, uint32_t numVertices) {
            create(data, numVertices);
        }

        virtual ~VertexBuffer() {
            GLCALL(glDeleteBuffers(1, &bufferID));
        }

        void create(void* data, uint32_t numVertices) {
            GLCALL(glGenBuffers(1, &bufferID));
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
            if(data == nullptr) {
                GLCALL(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_DYNAMIC_DRAW));
            } else {
                GLCALL(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW));
            }

            GLCALL(glEnableVertexAttribArray(0));   //pos
            GLCALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, pos)));

            GLCALL(glEnableVertexAttribArray(1));   //color
            GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, color)));

            GLCALL(glEnableVertexAttribArray(2));   //tex cords
            GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, texCord)));
            /*GLCALL(glEnableVertexAttribArray(3));   //tex index
            GLCALL(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, index)));
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));*/

        }

        void deleteBuffer() {
            GLCALL(glDeleteBuffers(1, &bufferID));
        }

        void addSubData(void* data, uint64_t size) {
            GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(Vertex), data));
        }

        void bind() {
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
        }

        void unbind() {
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

    private:
        GLuint bufferID;
    };
}