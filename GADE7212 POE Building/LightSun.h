#pragma once
#include <glm/glm.hpp>
#include "Light.h"

class LightSun : Light
{
	// Store IDs for lights
private:
	unsigned int id = 0; // Positive int value -> all lights have incremented ID when created e.g. 1,2,3,4,etc

	glm::vec4 diffuse = { 1,1,1,1 }; // Mat colour
	glm::vec4 ambient = { 1,1,1,1 }; // Overall colour
	glm::vec4 specular = { 1,1,1,1 }; // Reflection colour
	glm::vec3 direction = { 1,1,1 }; //light direction

public:
	static unsigned int currentID;

	LightSun();

	void init();

	void setDiffuse(glm::vec4 value);
	void setAmbient(glm::vec4 value);
	void setSpecular(glm::vec4 value);

	// Enable / Disable Lights
	void enableSun();
	void disableSun();
};