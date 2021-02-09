#pragma once

#include <GL/glew.h>
#include "../FileIOsystem//Filestream.h"
#include "../UserNotificationSystem/UserNotificationSystem.h"
#include "GLError.h"
#include <glm/glm.hpp>

namespace gfx {
    struct Shader {
        Shader() = default;
        Shader(const char* filePathVertex, const char* filepathFragment) {
            create(filePathVertex, filepathFragment);
        }

        void create(const char* filePathVertex, const char* filepathFragment) {
            shaderID = createShader(filePathVertex, filepathFragment);
        }

        virtual ~Shader() {
            glDeleteProgram(shaderID);
        }

        void bind() {
            glUseProgram(shaderID);
        }

        void unbind() {
            glUseProgram(0);
        }

        GLuint getID() {
            return shaderID;
        }

        GLuint getUniformLocation(const char* uniform) {
            return glGetUniformLocation(shaderID, uniform);
        }

        bool setUniform1i(const char* uniform, int data) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform1i(location, data);
                return true;
            }
            return false;
        }

        bool setUniform2i(const char* uniform, int i1, int i2) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform2i(location, i1, i2);
                return true;
            }
            return false;
        }

        bool setUniform3i(const char* uniform, int i1, int i2, int i3) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform3i(location, i1, i2, i3);
                return true;
            }
            return false;
        }

        bool setUniform4i(const char* uniform, int i1, int i2, int i3, int i4) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform4i(location, i1, i2, i3, i4);
                return true;
            }
            return false;
        }

        bool setUniform1f(const char* uniform, float f) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform1f(location, f);
                return true;
            }
            return false;
        }

        bool setUniform2f(const char* uniform, glm::vec2 data) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform2f(location, data.x, data.y);
                return true;
            }
            return false;
        }

        bool setUniform3f(const char* uniform, glm::vec3 data) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform3f(location, data.x, data.y, data.z);
                return true;
            }
            return false;
        }

        bool setUniform4f(const char* uniform, glm::vec4 data) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform4f(location, data.x, data.y, data.z, data.w);
                return true;
            }
            return false;
        }

        bool setUniform1d(const char* uniform, double d) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform1d(location, d);
                return true;
            }
            return false;
        }

        bool setUniform2d(const char* uniform, double d1, double d2) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform2d(location, d1, d2);
                return true;
            }
            return false;
        }

        bool setUniform3d(const char* uniform, double d1, double d2, double d3) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform3d(location, d1, d2, d3);
                return true;
            }
            return false;
        }

        bool setUniform4d(const char* uniform, double d1, double d2, double d3, double d4) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniform4d(location, d1, d2, d3, d4);
                return true;
            }
            return false;
        }

        bool setUniformMatrix4fv(const char* uniform, int count, GLboolean transpose, glm::mat4& matrix) {
            int location =  glGetUniformLocation(shaderID, uniform);
            if(location != -1) {
                glUniformMatrix4fv(location, count, transpose, &matrix[0][0]);
                return true;
            }
            return false;
        }

    private:
        GLuint compile(char* source, GLenum type) {
            GLuint id =  GLCALL(glCreateShader(type));
            glShaderSource(id, 1, &source, nullptr);
            glCompileShader(id);
            int result;
            glGetShaderiv(id, GL_COMPILE_STATUS, &result);
            if(result != GL_TRUE) {
                int length = 0;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                char* message = new char[length];
                glGetShaderInfoLog(id, length, &length, message);
                uns::log<uns::CRIT>("Shader Compilation Error: $c", message);
                delete[](message);
                return 0;
            }
            return id;
        }

        GLuint createShader(const char* filePathVertex, const char* filepathFragment) {
            char* vertex = fios::readFile(filePathVertex);
            char* fragment = fios::readFile(filepathFragment);
            GLuint program =  glCreateProgram();
            GLuint vs = compile(vertex, GL_VERTEX_SHADER);
            GLuint fs = compile(fragment, GL_FRAGMENT_SHADER);

            glAttachShader(program, vs);
            glAttachShader(program, fs);
            glLinkProgram(program);
            glDetachShader(program, vs);
            glDetachShader(program, fs);
            glDeleteShader(vs);
            glDeleteShader(fs);

            return program;
        }

        GLuint shaderID;
    };
}
