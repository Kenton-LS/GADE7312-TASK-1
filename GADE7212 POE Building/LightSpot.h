#pragma once
#include <glm/glm.hpp>
#include "Light.h"

class LightSpot : public Light
{
private:
	unsigned int id = 0; // Positive int value -> all lights have incremented ID when created e.g. 1,2,3,4,etc

	glm::vec4 position = { 0,0,0,1 }; // 4th value: 0 = directional light, 1 = point light
	// OR glm::vec4 position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::vec4 diffuse = { 1,1,1,1 }; // Mat colour
	glm::vec4 ambient = { 1,1,1,1 }; // Overall colour

	glm::vec3 direction = { 1,1,1 }; //light direction
	float spot_cutoff;
	float spot_exponent;

public:
	static unsigned int SPcurrentID;

	LightSpot();

	void init();

	void setPosition(glm::vec4 value);
	void setDiffuse(glm::vec4 value);
	void setAmbient(glm::vec4 value);

	void setDirection(glm::vec3 value);
	void setCutoff(float value);
	void setExponent(float value);

	void enableSpot();
	void disableSpot();
};