#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>
#include <GL/glew.h>
#include "ReferenceCounter.h"


class TextureData : public ReferenceCounter
{
private:
	GLenum textureTarget;
	GLuint textureID;

	void operator =(TextureData& textureData) {}

public:
	TextureData(GLenum textureTarger);
	~TextureData();

	GLenum getTextureTarget();
	GLuint getTextureID();
};

class Texture
{
private:
	static std::map<std::string, TextureData*> resourceMap;

	TextureData* textureData;
	std::string fileName;

	void initTexture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter);

	void operator =(Texture& texture) {}

public:
	Texture(std::string fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
	Texture(int width, int height, unsigned char* data, GLenum textureTargert, GLfloat filter);
	~Texture();

	void bind(int uint);
};

#endif