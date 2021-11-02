#pragma once
#include "Texture.h"
#include <nlohmann/json.hpp>
#include <vector> // Not actually a Vector like c# -> is instead a LIST
#include <GL/freeglut.h>

using json = nlohmann::json;
using namespace std;

class TextureLibrary
{
public:
	TextureLibrary(string path);
	~TextureLibrary();

	std::vector<const char *> storedTextures; // Array of textures themselves (paths)
	std::vector<std::string> storedTextureNames; // Array of texture names as strings
	int storedTextureCount;

private:
	GLuint id; // Private integer for display lists
	void load(string path);
	json textureLibraryJSON;
	vector<Texture*> textures; // Load JSON, and dynamically create texture -> on dynamically allocated memory (on stack, not heap)

	void parseTextureLibrary();
};