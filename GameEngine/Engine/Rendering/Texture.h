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

	void initTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* basicFormat, bool clamp);
	void initRenderTargets(GLenum* attachments);

	void operator =(TextureData& textureData) {}

public:
	TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* basicFormat, bool clamp, GLenum* attachments);
	~TextureData();

	void bind(int textureNum);
	void bindAsRenderTarget();

	int getWidth();
	int getHeight();
};

class Texture
{
private:
	static std::map<std::string, TextureData*> resourceMap;

	TextureData* textureData;
	std::string fileName;

	void operator =(Texture& texture) {}

public:
	Texture(std::string fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum basicFormat = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum basicFormat = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	~Texture();

	void bind(int uint);
	void bindAsRenderTarget();

	int getWidth();
	int getHeight();
};

#endif