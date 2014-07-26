#include <iostream>
#include <cassert>
#include "stb_image.h"
#include <GL/glew.h>
#include "Texture.h"

std::map<std::string, TextureData*> Texture::resourceMap;

TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* basicFormat, bool clamp, GLenum* attachments)
{
	m_textureID = new GLuint[numTextures];
	m_numTextures = numTextures;
	m_textureTarget = textureTarget;
	m_width = width;
	m_height = height;
	m_framebuffer = 0;
	m_renderbuffer = 0;

	initTextures(data, filters, internalFormat, basicFormat, clamp);
	initRenderTargets(attachments);
}

TextureData::~TextureData()
{
	if(*m_textureID)
	{
		glDeleteTextures(m_numTextures, m_textureID);
	}

	if(m_textureID)
	{
		delete[] m_textureID;
	}

	if(m_framebuffer)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
	}

	if(m_renderbuffer)
	{
		glDeleteRenderbuffers(1, &m_renderbuffer);
	}
}

void TextureData::initTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* basicFormat, bool clamp)
{
	glGenTextures(m_numTextures, m_textureID);

	for(int i = 0; i < m_numTextures; i++)
	{
		glBindTexture(m_textureTarget, m_textureID[i]);

		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);

		if(clamp)
		{
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, basicFormat[i], GL_UNSIGNED_BYTE, data[i]);
	}
}

void TextureData::initRenderTargets(GLenum* attachments)
{
	if(attachments == 0)
	{
		return;
	}

	GLenum drawBuffers[32];
	bool hasDepth = false;

	for(int i = 0; i < m_numTextures; i++)
	{
		if(attachments[i] == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else
		{
			drawBuffers[i] = attachments[i];
		}

		if(attachments[i] == GL_NONE)
		{
			continue;
		}
		
		if(m_framebuffer == 0)
		{
			glGenFramebuffers(1, &m_framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		}
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], m_textureTarget, m_textureID[i], 0);
	}

	if(m_framebuffer == 0)
	{
		return;
	}

	if(!hasDepth)
	{
		glGenRenderbuffers(1, &m_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	}

	glDrawBuffers(m_numTextures, drawBuffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureData::bind(int textureNum)
{
	glBindTexture(m_textureTarget, m_textureID[textureNum]);
}

void TextureData::bindAsRenderTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, m_width, m_height);
}

int TextureData::getWidth()
{
	return m_width;
}

int TextureData::getHeight()
{
	return m_height;
}

Texture::Texture(std::string fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum basicFormat, bool clamp, GLenum attachment)
{
	this->fileName = fileName;
	textureData = 0;

	std::map<std::string, TextureData*>::const_iterator it = resourceMap.find(fileName);

	if(it != resourceMap.end())
	{
		textureData = it->second;
		textureData->addReference();
	}
	else
	{
		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(("./res/textures/" + fileName).c_str(), &x, &y, &bytesPerPixel, 4);

		if(data == NULL)
		{
			std::cerr << "Unable to load texture: " << fileName << std::endl;
		}

		textureData = new TextureData(textureTarget, x, y, 1, &data, &filter, &internalFormat, &basicFormat, clamp, &attachment);
		stbi_image_free(data);

		resourceMap.insert(std::pair<std::string, TextureData*>(fileName, textureData));
	}
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum basicFormat, bool clamp, GLenum attachment)
{
	fileName = "";
	textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &basicFormat, clamp, &attachment);
}

Texture::~Texture()
{
}

void Texture::bind(int unit)
{
	assert(unit >= 0 && unit < 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	textureData->bind(0);
}

void Texture::bindAsRenderTarget()
{
	textureData->bindAsRenderTarget();
}

int Texture::getWidth()
{
	return textureData->getWidth();
}

int Texture::getHeight()
{
	return textureData->getHeight(); 
}