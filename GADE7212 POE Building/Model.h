#pragma once
#include <nlohmann/json.hpp>
#include <vector> 
#include <glm/glm.hpp>

#include "GameObject.h"

using json = nlohmann::json;
using namespace std;

class Model : public GameObject
{
public:
	Model(json modelJSON);
	~Model();
private:
	glm::vec3 position;
	vector<GameObject*> gameObjects;

	void generate(json modelJSON);
};