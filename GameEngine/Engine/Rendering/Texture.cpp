#include <iostream>
#include <cassert>
#include "stb_image.h"
#include <GL/glew.h>
#include "Texture.h"

std::map<std::string, TextureData*> Texture::resourceMap;

TextureData::TextureData(GLenum textureTarget)
{
	glGenTextures(1, &textureID);
	this->textureTarget = textureTarget;
}

TextureData::~TextureData()
{
	if(textureID)
	{
		glDeleteTextures(1, &textureID);
	}
}

GLenum TextureData::getTextureTarget()
{
	return textureTarget;
}

GLuint TextureData::getTextureID()
{
	return textureID;
}

Texture::Texture(std::string fileName, GLenum textureTarget, GLfloat filter)
{
	this->fileName = fileName;

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

		initTexture(x, y, data, textureTarget, filter);
		stbi_image_free(data);

		resourceMap.insert(std::pair<std::string, TextureData*>(fileName, textureData));
	}
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
	fileName = "";
	initTexture(width, height, data, textureTarget, filter);
}

Texture::~Texture()
{
}

void Texture::bind(int unit)
{
	assert(unit >= 0 && unit < 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(textureData->getTextureTarget(), textureData->getTextureID());
}

void Texture::initTexture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
	if(width > 0 && height > 0 && data != 0)
	{
		textureData = new TextureData(textureTarget);
		glBindTexture(textureTarget, textureData->getTextureID());

		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filter);

		glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}