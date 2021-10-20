#pragma once
#include "GameObject.h"
#include "Vertex.h"
#include "ModelPart.h"
#include <string>
#include <vector>
#include <GL/freeglut.h>

class ModelWhole : public GameObject // Called ModelWhole instead of Model as Model.cpp is a script already employed by the Level creation process
{
public:
	ModelWhole(std::string path, std::string name);
	~ModelWhole();
	float rotationOffset = 1.0f;
	float rotationSpeed = 1.0f;
private:
	void loadModel(std::string path, std::string name); // Values are passed into Load Model () method
	void drawGeometry() override; // Draw the stored list
	void generateGeometry(); // Draw actual model triangle geometry

	std::vector<ModelPart> parts; // List making up the model
	std::vector<Texture*> textures; // Each part has its own texture

	GLuint id;
};