#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>
#include <GL/glew.h>
#include "ReferenceCounter.h"


class TextureData : public ReferenceCounter
{
private:
	GLenum m_textureTarget;
	GLuint* m_textureID;
	GLuint m_framebuffer;
	GLuint m_renderbuffer;
	int m_numTextures;
	int m_width;
	int m_height;

	void initTextures(unsigned char** data, GLfloat* filters);
	void initRenderTargets(GLenum* attachments);

	void operator =(TextureData& textureData) {}

public:
	TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* attachments);
	~TextureData();

	void bind(int textureNum);
	void bindAsRenderTarget();
};

class Texture
{
private:
	static std::map<std::string, TextureData*> resourceMap;

	TextureData* textureData;
	std::string fileName;

	void operator =(Texture& texture) {}

public:
	Texture(std::string fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR, GLenum attachment = GL_NONE);
	Texture(int width, int height, unsigned char* data, GLenum textureTargert, GLfloat filter, GLenum attachment = GL_NONE);
	~Texture();

	void bind(int uint);
	void bindAsRenderTarget();
};

#endif