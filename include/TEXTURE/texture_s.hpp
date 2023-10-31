/*
*	TEXTURE_S.HPP
*
*   Class Texture gets in charge of loading textures from files, 
*	set to their desired atributes and then storing them in the GPU.
*
*	Class constructor takes in the path to the texture file and the atributes
*	needed to be set to the texture, including if a y-axis flip is needed;
*   and deconstructor deletes the texture from the GPU.
*
*	Unbind() and bind() functions for using the texture when needed.
* 
*	LoadFromFile() overwrites the current texture with a new one.
* 
*/

#ifndef TEXTURE_S_H
#define TEXTURE_S_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glad/glad.h>

#include <iostream>


class Texture
{
private:

	GLuint ID;
	GLenum type;
	int textureUnit;
	GLenum wrapTSMinMag_filters[4];
	int width, height, nrChannels;


public:

	Texture(const char* texturePath, GLenum type, int textureUnit, GLenum wrapTSMinMag_filters[], bool flip = true)
	{
		this->ID = -1;
		this->type = type;
		this->textureUnit = textureUnit;
		for (int i = 0; i < 4; i++) { this->wrapTSMinMag_filters[i] = wrapTSMinMag_filters[i]; }
		loadFromFile(texturePath, flip);
	}

	~Texture() 
	{
		glDeleteTextures(1, &this->ID);
	}

	inline GLuint getID() const { return this->ID; }
	inline int getTextureUnit() const { return this->textureUnit; }
	inline GLenum getFilter(int filter_pos) const { return this->wrapTSMinMag_filters[filter_pos]; }

	void modifyFilters(GLenum wrapTSMinMag_filters[])
	{
		this->bind();

		for (int i = 0; i < 4; i++) { this->wrapTSMinMag_filters[i] = wrapTSMinMag_filters[i]; }

		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, this->wrapTSMinMag_filters[0]);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, this->wrapTSMinMag_filters[1]);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, this->wrapTSMinMag_filters[2]);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, this->wrapTSMinMag_filters[3]);

		this->unbind();
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->ID);
	}
	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(this->type, 0);
		glDisable(this->type);
	}

	void loadFromFile(const char* texturePath, bool flip)
	{
		// If there is already a texture loaded delete it first
		if (this->ID != (GLuint) -1)
		{
			glDeleteTextures(1, &this->ID);
			this->ID = -1;
		}

		// Load the texture and flip y-axis if needed
		if (flip) stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

		// Generate and bind the texture to be able to modify it
		glGenTextures(1, &this->ID);
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->ID);

		// Set the texture wrapping and filtering options
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, this->wrapTSMinMag_filters[0]);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, this->wrapTSMinMag_filters[1]);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, this->wrapTSMinMag_filters[2]);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, this->wrapTSMinMag_filters[3]);


		if (data)
		{
			// Set format depending on the number of channels
			GLenum format = GL_RGB;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			// Attach the texture to the currently bound framebuffer object
			glTexImage2D(this->type, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(this->type);

		}
		else
		{
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED" << '\n';
		}

		glActiveTexture(0);
		glBindTexture(this->type, 0);
		stbi_image_free(data);
	}

};

#endif // !TEXTURE_S_H