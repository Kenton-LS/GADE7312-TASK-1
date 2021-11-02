#include <GL/freeglut.h>
#include <fstream>
#include <iostream>

#include "TextureLibrary.h"
#include "Texture.h"

TextureLibrary::TextureLibrary(string path)
{
	load(path);
}

TextureLibrary::~TextureLibrary()
{
	for (Texture * t : textures) // Dynamically delete all list members through the pointer itself
	{
		delete t;
	}
}

void TextureLibrary::load(string path)
{
	string jsonString = "";
	string line;

	// Read into a program / stream from a file, also: parses in path you want to load levelFile(path)
	ifstream textureLibraryFile(path); // Check if level file is open

	if (textureLibraryFile.is_open())
	{
		while (getline(textureLibraryFile, line))
		{
			jsonString += line;
			cout << line << endl;
		}
		textureLibraryFile.close(); // Done, close level file 
	}
	else // if for some reason, cannot open file
	{
		cerr << "Error: unable to open level file: " << path << endl;
		return; // Return out of load method -> can't build level if no file
	}

	// Continue loading level from file if no problems
	textureLibraryJSON = json::parse(jsonString);
	parseTextureLibrary();
}

void TextureLibrary::parseTextureLibrary()
{
	storedTextureCount = textureLibraryJSON["storedTextures"].size();

	for (int i = 0; i < storedTextureCount; i++)
	{
		json storedTexture = textureLibraryJSON["storedTextures"][i]; // Grab each JSON texture object individually

		// Texture Path
		string path = textureLibraryJSON["storedTextures"][i]["texture"]; // Convert string path to const char* array
		const char* charPath = path.c_str();

		//Texture* texture = new Texture(charPath);
		storedTextures.push_back(charPath); // IE Same as C# "Add" function to a list

		// Texture Name
		string name = textureLibraryJSON["storedTextures"][i]["id"]; // Get id for names string array
		storedTextureNames.push_back(name);
	}

	id = glGenLists(1); // Generate a single list (1) / collection of geometry

	glEndList();
}