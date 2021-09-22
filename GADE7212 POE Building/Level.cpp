#include "Level.h"
#include "Room.h"
#include <GL/freeglut.h>
#include <fstream>
#include <iostream>

Level::Level(string path)
{
	load(path);
}

Level::~Level()
{
	for (GameObject * g : gameObjects) // Dynamically delete all list members through the pointer itself
	{
		delete g;
	}
}

void Level::load(string path)
{
	string jsonString = "";
	string line;

	// Read into a program / stream from a file, also: parses in path you want to load levelFile(path)
	ifstream levelFile(path); // Check if level file is open

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
		cerr << "Error: unable to open level file: " << path << endl;
		return; // Return out of load method -> can't build level if no file
	}

	// Continue loading level from file if no problems
	levelJSON = json::parse(jsonString);
	parseLevel();
}

void Level::parseLevel()
{
	int roomCount = levelJSON["rooms"].size();
	roomPadding = levelJSON["roomPadding"];
	floorTexture = levelJSON["floorTexture"];

	for (int i = 0; i < roomCount; i++)
	{
		// Grab each JSON room object individually
		json roomJSON = levelJSON["rooms"][i]; // Single room

		// Room
		Room* room = new Room(roomJSON, roomPadding, floorTexture); // Parse the room JSON and room padding (space between walls -> illusion of thickness to the wall)
		gameObjects.push_back(room); // IE Same as C# "Add" function to a list
	}

	id = glGenLists(1); // Generate a single list (1) / collection of geometry

	glNewList(id, GL_COMPILE); // Now specify geometry that goes with this list
	{
		for (GameObject* g : gameObjects) // C++ for loop (exquisite)
		{
			g->draw();
		}
	}
	glEndList();
}

void Level::drawGeometry()
{
	glCallList(id);
}