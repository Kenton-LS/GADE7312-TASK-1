#include <GL/freeglut.h>
#include <fstream>
#include <iostream>

#include "Loader.h"
#include "Model.h"

Loader::Loader(string pathModel)
{
	load(pathModel);
}

Loader::~Loader()
{
	for (GameObject * g : gameObjects) // Dynamically delete all list members through the pointer itself
	{
		delete g;
	}
}

void Loader::load(string pathModel)
{
	string jsonString = "";
	string line;

	// Read into a program / stream from a file, also: parses in path you want to load levelFile(path)
	ifstream levelFile(pathModel); // Check if level file is open

	if (levelFile.is_open())
	{
		while (getline(levelFile, line))
		{
			jsonString += line;
			cout << line << endl;
		}
		levelFile.close(); // Done, close level file 
	}
	else // if for some reason, cannot open file
	{
		cerr << "Error: unable to open level file: " << pathModel << endl;
		return; // Return out of load method -> can't build level if no file
	}

	// Continue loading level from file if no problems
	modelJSON = json::parse(jsonString);
	parseModel();
}

void Loader::parseModel()
{
	int modelCount = modelJSON["models"].size();

	for (int i = 0; i < modelCount; i++)
	{
		// Grab each JSON room object individually
		json modelJSON = modelJSON["models"][i]; // Single room

		Model* model = new Model(modelJSON); // Parse the room JSON and room padding (space between walls -> illusion of thickness to the wall)
		gameObjects.push_back(model); // IE Same as C# "Add" function to a list
	}

	id = glGenLists(1); // Generate a single list (1) / collection of geometry

	/*glNewList(id, GL_COMPILE); // Now specify geometry that goes with this list
	{
		for (GameObject* g : gameObjects) // C++ for loop (exquisite)
		{
			g->draw();
		}
	}
	glEndList();*/
}