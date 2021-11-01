#pragma once
#include <nlohmann/json.hpp>
#include <vector> // Not actually a Vector like c# -> is instead a LIST
#include <glm/glm.hpp>

#include "GameObject.h"

using json = nlohmann::json;

using namespace std;

class Room : public GameObject
{
public:
	Room(json roomJSON, json modelJSON, float roomPadding, string floorTexture, string roofTexture, bool hasRoof, bool hasFloor, bool hasDoors);
	~Room();
	float vertexCount; // TASK 2 
private:
	glm::vec3 position;
	vector<GameObject*> gameObjects; 

	void generate(json roomJSON, json modelJSON, float roomPadding, string floorTexture, string roofTexture, bool hasRoof, bool hasFloor, bool hasDoors);
	void drawGeometry() override;

	void assignValues(json roomJSON, int jsonIndex);
	/*void drawHole(json roomJSON, int jsonIndex, bool southFace,
		glm::vec3 vStart, glm::vec3 verticalDirection, glm::vec3 vy, float roomPadding, glm::vec3 colour, glm::vec3 normal,
		const char *  textureCharW, vector<GameObject*> gameObjects);*/
};