#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <map>
#include <string>

#include "../Core/ReferenceCounter.h"

class TextureData : public ReferenceCounter
{
private:
	GLuint* m_textureID;
	GLenum m_textureTarget;
	GLuint m_framebuffer;
	GLuint m_renderbuffer;
	int m_numTextures;
	int m_width;
	int m_height;

	TextureData(TextureData& other) {}

	void initTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* basicFormat, bool clamp);
	void initRenderTargets(GLenum* attachments);

	void operator =(TextureData& textureData) {}

public:
	TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* basicFormat, bool clamp, GLenum* attachments);
	virtual ~TextureData();

	void bind(int textureNum) const;
	void bindAsRenderTarget() const;

	int getWidth() const;
	int getHeight() const;
	GLuint* getId() const;
};

class Texture
{
private:
	static std::map<std::string, TextureData*> resourceMap;

	TextureData* m_textureData;
	std::string m_fileName;

public:
	Texture(std::string fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum basicFormat = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum basicFormat = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	Texture(const Texture& texture);
	virtual ~Texture();

	void operator =(Texture texture);

	void bind(int uint) const;
	void bindAsRenderTarget() const;

	int getWidth() const;
	int getHeight() const;
	GLuint* getId() const;
};

#endif