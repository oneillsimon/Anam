#include <iostream>
#include <cassert>
#include "stb_image.h"
#include "../Core/Math3D.h"
#include "../Core/Profiling.h"
#include "Texture.h"

std::map<std::string, TextureData*> Texture::resourceMap;

TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* basicFormat, bool clamp, GLenum* attachments)
{
	m_textureID = new GLuint[numTextures];
	m_numTextures = numTextures;
	m_textureTarget = textureTarget;

#if PROFILING_SET_2x2_TEXTURE == 0
	m_width = width;
	m_height = height;
#else
	m_width = 2;
	m_height = 2;
#endif

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
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, basicFormat[i], GL_UNSIGNED_BYTE, data[i]);

		if(filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
		   filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
		   filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
		   filters[i] == GL_LINEAR_MIPMAP_LINEAR)
		{
			glGenerateMipmap(m_textureTarget);
			GLfloat maxAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
		}
		else
		{
			glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}
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

void TextureData::bind(int textureNum) const
{
	glBindTexture(m_textureTarget, m_textureID[textureNum]);
}

void TextureData::bindAsRenderTarget() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);

#if PROFILING_SET_1x1_VIEWPORT == 0
	glViewport(0, 0, m_width, m_height);
#else
	glViewport(0, 0, 1, 1);
#endif
}

int TextureData::getWidth() const
{
	return m_width;
}

int TextureData::getHeight() const
{
	return m_height;
}

Texture::Texture(std::string fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum basicFormat, bool clamp, GLenum attachment)
{
	m_fileName = fileName;

	std::map<std::string, TextureData*>::const_iterator it = resourceMap.find(fileName);

	if(it != resourceMap.end())
	{
		m_textureData = it->second;
		m_textureData->addReference();
	}
	else
	{
		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(("./res/textures/" + fileName).c_str(), &x, &y, &bytesPerPixel, 4);

		if(data == NULL)
		{
			std::cerr << "Unable to load texture: " << fileName << std::endl;
		}

		GLfloat i = GL_LINEAR_MIPMAP_LINEAR;
		GLfloat j = filter;
		m_textureData = new TextureData(textureTarget, x, y, 1, &data, &i, &internalFormat, &basicFormat, clamp, &attachment);
		stbi_image_free(data);

		resourceMap.insert(std::pair<std::string, TextureData*>(fileName, m_textureData));
	}
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum basicFormat, bool clamp, GLenum attachment)
{
	m_fileName = "";
	m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &basicFormat, clamp, &attachment);
}

Texture::Texture(const Texture& texture) :
	m_textureData(texture.m_textureData),
	m_fileName(texture.m_fileName)
{
	m_textureData->addReference();
}

Texture::~Texture()
{
	if(m_textureData && m_textureData->removeReference())
	{
		if(m_fileName.length() > 0)
		{
			resourceMap.erase(m_fileName);
		}

		delete m_textureData;
	}
}

void Texture::bind(int unit) const
{
	assert(unit >= 0 && unit < 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData->bind(0);
}

void Texture::bindAsRenderTarget() const
{
	m_textureData->bindAsRenderTarget();
}

int Texture::getWidth() const
{
	return m_textureData->getWidth();
}

int Texture::getHeight() const
{
	return m_textureData->getHeight(); 
}

void Texture::operator =(Texture t)
{
	char* temp[sizeof(Texture) / sizeof(char)];
	memcpy(temp, this, sizeof(Texture));
	memcpy(this, &t, sizeof(Texture));
	memcpy(&t, temp, sizeof(Texture));
}