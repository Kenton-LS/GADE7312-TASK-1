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
	Room(json roomJSON, float roomPadding, string floorTexture, string roofTexture, bool hasRoof, bool hasFloor, bool hasDoors);
	~Room();
private:
	glm::vec3 position;
	vector<GameObject*> gameObjects; 

	void generate(json roomJSON, float roomPadding, string floorTexture, string roofTexture, bool hasRoof, bool hasFloor, bool hasDoors);
	void drawGeometry() override;
};

