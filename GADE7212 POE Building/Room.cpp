#include <iostream>
#include "Room.h"
#include "Wall.h"
#include "Floor.h"
#include <glm/glm.hpp>
#include <GL/freeglut.h>

using namespace glm;

Room::Room(json roomJSON, float roomPadding)
{
	position = vec3(
		roomJSON["position"][0],
		roomJSON["position"][1],
		roomJSON["position"][2]
	);

	generate(roomJSON, roomPadding);
}


Room::~Room()
{
	for (GameObject * g : gameObjects) // Delete all gameobjects we created
	{
		delete g;
	}
}

void Room::generate(json roomJSON, float roomPadding)
{
	float width = roomJSON["width"];
	float height = roomJSON["height"];
	float length = roomJSON["length"];

	// Next: Create 3 vectors -> need room's length, width, height as vectors
	// Need to get as vectors, while taking room padding into consideration
	// Thus if walls are 0.2 thick -----> width - roomPadding * 2, etc (see below)

	vec3 vx = vec3(width - (roomPadding * 2), 0, 0);
	vec3 vy = vec3(0, height, 0);
	vec3 vz = vec3(0, 0, length - (roomPadding * 2));

	vec3 vStart = vec3(-width / 2.0f + roomPadding, 0, length / 2.0f - roomPadding); // The point at which our walls start

	for (int i = 0; i < 5; i++)
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
		case 4:
			verticalDirection = -vx;
			break;
		}

		// Get normal of each face we are drawing
		vec3 normal;
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
		case 4:
			normal = vec3(0, 1, 0);
			break;
		}

		// Could alternatively put wall colour parameter in the JSON file (in room properties)
		vec3 colour = vec3(1, 1, 1); // White placeholder for now

		if (i < 4)
		{
			Wall * wall = new Wall(vStart, verticalDirection, vy, colour, normal); // Parameters: (vec3 where we start, the vertical direction, 
																			  //the up vector to get wall height, the colour vec, and the normal vec)
			gameObjects.push_back(wall);
		}
		else
		{
			Floor * floor = new Floor(vStart, width - (roomPadding * 2), height, length - (roomPadding * 2), colour, normal);

			gameObjects.push_back(floor);
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