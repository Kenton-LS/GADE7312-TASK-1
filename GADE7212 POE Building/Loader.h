#pragma once
#include "GameObject.h"
#include <nlohmann/json.hpp>
#include <vector> 
#include <GL/freeglut.h>

using json = nlohmann::json;
using namespace std;

class Loader : public GameObject
{
public:
	Loader(string pathModel);
	~Loader();
private:
	GLuint id;

	void load(string pathModel);
	json modelJSON;
	vector<GameObject*> gameObjects; 

	void parseModel();
};