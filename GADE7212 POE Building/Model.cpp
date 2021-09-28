#include <iostream>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

#include "Model.h"

using namespace glm;

Model::Model(json modelJSON)
{
	position = vec3(
		modelJSON["position"][0],
		modelJSON["position"][1],
		modelJSON["position"][2]
	);

	generate(modelJSON);
}

Model::~Model()
{
	for (GameObject * g : gameObjects)
	{
		delete g;
	}
}

void Model::generate(json modelJSON)
{
	for (auto &array : modelJSON["models"]) 
	{
		string id = modelJSON["id"];
		cout << "Model ID: " << id << " has been read by Model.cpp & Loader.cpp classes." << endl;
	}
}