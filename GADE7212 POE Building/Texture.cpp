#include "Texture.h"
#include <GL/freeglut.h>
#include <iostream>

// STB
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* path)
{
	loadTexture(path);
}

Texture::~Texture()
{
	// Release the memory the image has taken up after a texture is destroyed
	stbi_image_free(image);
}

void Texture::use()
{
	if (!loaded)
	{
		std::cout << "WARNING: Trying to use an incorrectly loaded texture" << std::endl;
		return;
	}

	if (image)
	{
		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
}

void Texture::loadTexture(const char* path)
{
	// **First tell OGL how to interpret the data we are loading it**
	// WRAP_S = X axis, WRAP_T = Y axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Filter for 2 params -> 
	// GL_NEAREST takes nearest pixel value, keeping blocky aesthetic --- Pixel Art
	// GL_LINEAR on the other hand blends / smooths pixels (good for faces) --- Generic Images
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true); // Prevents upside down image loading

	// **Load the image at a specific location**
	// Send pointer to width heigh channel variables (in header file)
	image = stbi_load(path, &width, &height, &channels, STBI_rgb);

	// If image is populated
	if (image)
	{
		loaded = true;
		std::cout << "NOTICE: Texture loading succeeded: " << path << std::endl;
		std::cout << width << " x " << height << " y " << std::endl;
	}
	else
	{
		std::cout << "WARNING: Texture loading failed: " << path << std::endl;
	}
}