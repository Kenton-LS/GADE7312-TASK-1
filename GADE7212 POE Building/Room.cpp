#include <iostream>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

#include "Room.h"
#include "Wall.h"
#include "Floor.h"
#include "Roof.h"
#include "Hole.h"
#include "Door.h"

using namespace glm;

Room::Room(json roomJSON, float roomPadding, string floorTexture, string roofTexture)
{
	position = vec3(
		roomJSON["position"][0],
		roomJSON["position"][1],
		roomJSON["position"][2]
	);

	generate(roomJSON, roomPadding, floorTexture, roofTexture);
}

Room::~Room()
{
	for (GameObject * g : gameObjects) // Delete all gameobjects we created
	{
		delete g;
	}
}

void Room::generate(json roomJSON, float roomPadding, string floorTexture, string roofTexture)
{
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	float width = roomJSON["width"];
	float height = roomJSON["height"];
	float length = roomJSON["length"];

	// For Hole Walls
	string face; 
	bool southFace; 
	float holeWidth, holeHeight, offset;

	// For Door Walls
	bool northFace; // (Also uses above holewidth, holeheight, and offset for door dimensions)

	for (auto &array : roomJSON["holes"]) // Manually get HOLE array values from inside each room
	{
		face = array["face"];
		holeWidth = array["holeWidth"];
		holeHeight = array["holeHeight"];
		offset = array["offset"];
	}
	// End Hole Walls
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	string textureStringW = roomJSON["texture"]; // Get Wall Texture from JSON as string, convert to const char array
	cout << textureStringW << endl;
	const char * textureCharW = textureStringW.c_str();

	string textureStringF = floorTexture; // Get Floor Texture
	cout << textureStringF << endl;
	const char * textureCharF = textureStringF.c_str();

	string textureStringR = roofTexture; // Get Roof Texture
	cout << textureStringR << endl;
	const char * textureCharR = textureStringR.c_str();
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// Next: Create 3 vectors -> need room's length, width, height as vectors
	// Need to get as vectors, while taking room padding into consideration
	// Thus if walls are 0.2 thick -----> width - roomPadding * 2, etc (see below)

	vec3 vx = vec3(width - (roomPadding * 2), 0, 0);
	vec3 vy = vec3(0, height, 0);
	vec3 vz = vec3(0, 0, length - (roomPadding * 2));

	vec3 vStart = vec3(-width / 2.0f + roomPadding, 0, length / 2.0f - roomPadding); // The point at which our walls start
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	for (int i = 0; i < 6; i++)
	{
		vec3 verticalDirection;
		switch (i) // Get direction to move in box
		{
		case 0:
			verticalDirection = vx;
			break;
		case 1:
			verticalDirection = -vz;
			break;
		case 2:
			verticalDirection = -vx;
			break;
		case 3:
			verticalDirection = vz;
			break;
		case 4: // Floor
			verticalDirection = vy;
			break;
		case 5: // Roof
			verticalDirection = -vx;
			break;
		}

		vec3 normal; // Get normal of each face we are drawing
		switch (i) // Get direction to move in box
		{
		case 0:
			normal = vec3(0, 0, -1); // Make sure normals of each wall face inward (so player can see wall geometry and textures)
			break;
		case 1:
			normal = vec3(-1, 0, 0);
			break;
		case 2:
			normal = vec3(0, 0, 1);
			break;
		case 3:
			normal = vec3(1, 0, 0);
			break;
		case 4: // Floor
			normal = vec3(0, 1, 0);
			break;
		case 5: // Roof
			normal = vec3(0, 1, 0);
			break;
		}

		vec3 colour = vec3(1, 1, 1); // White placeholder for now -> could alternatively put in the JSON file (in room properties)
		switch (i)
		{
		case 0: // South Face - Wall / Hole?
		{
			if (roomJSON["holes"][0]["face"] == "s") // Wall with hole 
			{			
				face = roomJSON["holes"][0]["face"];
				holeWidth = roomJSON["holes"][0]["holeWidth"];
				holeHeight = roomJSON["holes"][0]["holeHeight"];
				offset = roomJSON["holes"][0]["offset"];

				southFace = true;
				Hole *hole = new Hole(vStart, verticalDirection, vy, southFace, offset, holeWidth, roomPadding, colour, normal);
				hole->setTexture(textureCharW);
				gameObjects.push_back(hole);
			}
			else if (roomJSON["holes"][1]["face"] == "s")
			{
				face = roomJSON["holes"][1]["face"];
				holeWidth = roomJSON["holes"][1]["holeWidth"];
				holeHeight = roomJSON["holes"][1]["holeHeight"];
				offset = roomJSON["holes"][1]["offset"];

				southFace = true;
				Hole *hole = new Hole(vStart, verticalDirection, vy, southFace, offset, holeWidth, roomPadding, colour, normal);
				hole->setTexture(textureCharW);
				gameObjects.push_back(hole);
			}
			else
			{
				Wall *wall = new Wall(vStart, verticalDirection, vy, colour, normal);
				wall->setTexture(textureCharW);
				gameObjects.push_back(wall);
			}
		}
		break;
		case 1: // East Face - Wall / Hole?
		{
			if (roomJSON["holes"][0]["face"] == "e")
			{
				face = roomJSON["holes"][0]["face"];
				holeWidth = roomJSON["holes"][0]["holeWidth"];
				holeHeight = roomJSON["holes"][0]["holeHeight"];
				offset = roomJSON["holes"][0]["offset"];

				southFace = false;
				Hole *hole = new Hole(vStart, verticalDirection, vy, southFace, offset, holeWidth, roomPadding, colour, normal);
				hole->setTexture(textureCharW);
				gameObjects.push_back(hole);
			}
			else if (roomJSON["holes"][1]["face"] == "e")
			{
				face = roomJSON["holes"][1]["face"];
				holeWidth = roomJSON["holes"][1]["holeWidth"];
				holeHeight = roomJSON["holes"][1]["holeHeight"];
				offset = roomJSON["holes"][1]["offset"];

				southFace = false;
				Hole *hole = new Hole(vStart, verticalDirection, vy, southFace, offset, holeWidth, roomPadding, colour, normal);
				hole->setTexture(textureCharW);
				gameObjects.push_back(hole);
			}
			else
			{
				Wall *wall = new Wall(vStart, verticalDirection, vy, colour, normal);
				wall->setTexture(textureCharW);
				gameObjects.push_back(wall);
			}
		}
		break;
		case 2: // North face - Wall / Door?
		{
			if (roomJSON["doors"][0]["face"] == "n")
			{
				face = roomJSON["doors"][0]["face"];
				holeWidth = roomJSON["doors"][0]["doorWidth"];
				holeHeight = roomJSON["doors"][0]["doorHeight"];
				offset = roomJSON["doors"][0]["offset"];

				northFace = true;
				Door *door = new Door(vStart, verticalDirection, vy, northFace, offset, holeWidth, roomPadding, colour, normal);
				door->setTexture(textureCharW);
				gameObjects.push_back(door);
			}
			else if (roomJSON["doors"][1]["face"] == "n")
			{
				face = roomJSON["doors"][1]["face"];
				holeWidth = roomJSON["doors"][1]["doorWidth"];
				holeHeight = roomJSON["doors"][1]["doorHeight"];
				offset = roomJSON["doors"][1]["offset"];

				northFace = true;
				Door *door = new Door(vStart, verticalDirection, vy, northFace, offset, holeWidth, roomPadding, colour, normal);
				door->setTexture(textureCharW);
				gameObjects.push_back(door);
			}
			else
			{
				Wall *wall = new Wall(vStart, verticalDirection, vy, colour, normal);
				wall->setTexture(textureCharW);
				gameObjects.push_back(wall);
			}
		}
		break;
		case 3: // West face - Wall / Door?
		{
			if (roomJSON["doors"][0]["face"] == "w")
			{
				face = roomJSON["doors"][0]["face"];
				holeWidth = roomJSON["doors"][0]["doorWidth"];
				holeHeight = roomJSON["doors"][0]["doorHeight"];
				offset = roomJSON["doors"][0]["offset"];

				northFace = false;
				Door *door = new Door(vStart, verticalDirection, vy, northFace, offset, holeWidth, roomPadding, colour, normal);
				door->setTexture(textureCharW);
				gameObjects.push_back(door);
			}
			else if (roomJSON["doors"][1]["face"] == "w")
			{
				face = roomJSON["doors"][1]["face"];
				holeWidth = roomJSON["doors"][1]["doorWidth"];
				holeHeight = roomJSON["doors"][1]["doorHeight"];
				offset = roomJSON["doors"][1]["offset"];

				northFace = false;
				Door *door = new Door(vStart, verticalDirection, vy, northFace, offset, holeWidth, roomPadding, colour, normal);
				door->setTexture(textureCharW);
				gameObjects.push_back(door);
			}
			else
			{
				Wall *wall = new Wall(vStart, verticalDirection, vy, colour, normal);
				wall->setTexture(textureCharW);
				gameObjects.push_back(wall);
			}
		}
		break;
		case 4: // Floor
		{
			/*Floor *floor = new Floor(vStart, width - (roomPadding * 2), height, length - (roomPadding * 2), colour, normal);

			//floor->setTexture(textureCharW); // To have Floor same Texture as Walls
			floor->setTexture(textureCharF); // To have Floor as Wolfenstein Style
			gameObjects.push_back(floor);*/
		}
		break;
		case 5: // Roof
		{
			/*Roof *roof = new Roof(vStart, width - (roomPadding * 2), height, length - (roomPadding * 2), colour, normal);

			//roof->setTexture(textureCharW); // To have Roof same Texture as Walls
			roof->setTexture(textureCharR); // To have Roof as Wolfenstein Style
			gameObjects.push_back(roof);*/
		}
		break;
		default: // Wall
		{
			// Parameters: (vec3 where we start, the vertical direction, the up vector to get wall height, the colour vec, and the normal vec)
			Wall *wall = new Wall(vStart, verticalDirection, vy, colour, normal);
			wall->setTexture(textureCharW);
			gameObjects.push_back(wall);
		}
		}
		vStart += verticalDirection;

		// To add doorways, rather just make a new Wall Class specializing with having a doorway in itself -> specify in JSON file the door's properties
		// Could use GL QUAD, or get away with doing it as a POLYGON (but be careful - it must be specified in order)

		// "length":3
		// "doors": [ 
		//    "front", "back"
		//  ]
	}
}

void Room::drawGeometry() // Also Henk recommends using OBJ files instead of FBX
{
	glPushMatrix();
	{
		glTranslatef(position.x, position.y, position.z);
		for (GameObject* g : gameObjects)
		{
			g->draw();
		}
	}
	glPopMatrix();
}

// REFERENCE
// For converting string to const char*
// https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
// For extracting values from a JSON array inside a JSON array
// https://newbedev.com/accessing-elements-from-nlohmann-json