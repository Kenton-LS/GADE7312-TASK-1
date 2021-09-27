#include "LightSpot.h"

#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp> // Used to point to values
#include <iostream>

class LightPoint
{
private:
	unsigned int id = 0; // Positive int value -> all lights have incremented ID when created e.g. 1,2,3,4,etc

	glm::vec4 position = { 0,0,0,1 }; // 4th value: 0 = directional light, 1 = point light
	// OR glm::vec4 position = glm::vec4(0.0, 0.0, 0.0, 1.0);

	glm::vec4 diffuse = { 1,1,1,1 }; // Mat colour
	glm::vec4 ambient = { 1,1,1,1 }; // Overall colour

	float constantA;
	float linearA;
	float quadraticA;

public:
	static unsigned int PcurrentID;

	LightPoint();

	void init();

	void setPosition(glm::vec4 value);
	void setDiffuse(glm::vec4 value);
	void setAmbient(glm::vec4 value);

	void setCAttenuation(float value);
	void setLAttenuation(float value);
	void setQAttenuation(float value);

	// Enable / Disable Lights
	void enablePoint();
	void disablePoint();
};