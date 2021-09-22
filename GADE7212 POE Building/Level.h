#pragma once
#include "GameObject.h"
#include <nlohmann/json.hpp>
#include <vector> // Not actually a Vector like c# -> is instead a LIST
#include <GL/freeglut.h>

using json = nlohmann::json;
using namespace std;

class Level : public GameObject
{
public:
	Level(string path);
	~Level();
private:
	// Private integer for display lists
	GLuint id;

	void load(string path);
	json levelJSON;
	float roomPadding;
	string floorTexture;
	vector<GameObject*> gameObjects; // Load JSON, and dynamically create gameobject -> on dynamically allocated memory (on stack, not heap)

	void parseLevel();
	void drawGeometry() override;
};