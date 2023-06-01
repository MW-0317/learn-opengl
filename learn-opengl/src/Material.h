#pragma once
#include <iostream>
#include <vector>

#include "stb.h"

class Color
{
public:
	float color[4];

	Color(float r, float g, float b, float a)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}

	int getSize()
	{
		return 4;
	}
};

class Texture
{
public:
	unsigned int ID;

	std::vector<float> texCoords;

	int width;
	int height;

	Texture(const char* texturePath, std::vector<float> texCoords)
	{
		this->texCoords = texCoords;

		int nrChannels;
		unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

		if (!data)
		{
			std::cerr << "TEXTURE::" << texturePath << "::FAILED_TO_LOAD";
		}

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLint format = 0;

		if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		else
		{
			std::cerr << "TEXTURE::UNHANDLED_FORMAT::" << nrChannels;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	void use()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void setWrap(GLint GL_wrapSetting)
	{
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_wrapSetting);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_wrapSetting);
	}

	int getSize()
	{
		return 2;
	}
};

class SpriteSheet : public Texture
{
public:
	unsigned int spriteWidth;
	unsigned int spriteHeight;

	SpriteSheet(const char* sheetPath, int nWidth, int nHeight, 
						std::vector<float> texCoords) : Texture(sheetPath, texCoords)
	{
		spriteWidth = (int)(width / nWidth);
		spriteHeight = (int)(height / nHeight);
	}

	void indexToImageCoords() {};
	void imageCoordsToTexCoords() {};
	void setTexCoords() {};
};

class Material
{
public:
	Texture* texture;
	Color* color;

	Material(Texture* tex = NULL, Color* col = new Color(0.0, 0.0, 0.0, 1.0))
	{
		texture = tex;
		color = col;
	}

	int getSize()
	{
		return color->getSize() + texture->getSize();
	}

	std::vector<float> getVertices()
	{

	}
};

