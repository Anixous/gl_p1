#include "Headers/texture.h"
#include "include/stb/stb_image.h"

Texture::Texture(std::string filePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {  
    type = texType;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(filePath.c_str(), &widthImg, &heightImg, &numColCh, STBI_rgb_alpha);

	// Generates an OpenGL texture object
	glGenTextures(1, &textureId);
	glActiveTexture(slot);
	glBindTexture(texType, textureId);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.shaderId, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::bind()
{
	glBindTexture(type, textureId);
}

void Texture::unbind()
{
	glBindTexture(type, 0);
}
void Texture::del()
{
	glDeleteTextures(1, &textureId);
}