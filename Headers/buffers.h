#ifndef BUFFERS_H
#define BUFFERS_H

#include "../include/glad/glad.h"

class VBO 
{
    public:
        GLuint vboId;
        VBO(GLfloat* vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
};
class VAO 
{
    public:
        GLuint vaoId;
        VAO();

        void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

        void Bind();
        void Unbind();
        void Delete();
};
class EBO 
{
    public:
	    GLuint eboId;
	    EBO(GLuint* indices, GLsizeiptr size);

	    void Bind();
	    void Unbind();
	    void Delete();
};
#endif