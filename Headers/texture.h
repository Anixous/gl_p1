#ifndef TEXTURE_H
#define TEXTURE_H

#include "../include/glad/glad.h"
#include <string>
#include "shader.h"

class Texture {
    public:
        GLuint textureId;
        GLenum type;

        Texture(std::string filePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

        void texUnit(Shader& shader, const char* uniform, GLuint unit);

        void bind();
        void unbind();
        void del();
};

#endif