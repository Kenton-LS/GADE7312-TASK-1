#include "LightSpot.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp> // Used to point to values
#include <iostream>

unsigned int LightSpot::SPcurrentID = 0;

LightSpot::LightSpot() {
	id = SPcurrentID; // Because it is a static variable (shared among all light objects), we can increment it and it will carry thru between lights
	SPcurrentID++;
}

void LightSpot::init()
{
	// Initialize the light and tell OpenGL we are going to use the light in the graphics API
	glEnable(GL_LIGHT1 + id);

	glLightfv(GL_LIGHT1 + id, GL_POSITION, glm::value_ptr(position)); // Convert vec4 to a float made of those 4 values
	glLightfv(GL_LIGHT1 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
	glLightfv(GL_LIGHT1 + id, GL_AMBIENT, glm::value_ptr(ambient));

	glLightf(GL_LIGHT1 + id, GL_SPOT_CUTOFF, spot_cutoff);
	glLightfv(GL_LIGHT1 + id, GL_SPOT_DIRECTION, glm::value_ptr(direction));
	glLightf(GL_LIGHT1 + id, GL_SPOT_EXPONENT, spot_exponent);
}

void LightSpot::setPosition(glm::vec4 value)
{
	position = value;
	glLightfv(GL_LIGHT1 + id, GL_POSITION, glm::value_ptr(position));
}
void LightSpot::setAmbient(glm::vec4 value)
{
	ambient = value;
	glLightfv(GL_LIGHT1 + id, GL_AMBIENT, glm::value_ptr(ambient));
}

void LightSpot::setDiffuse(glm::vec4 value)
{
	diffuse = value;
	glLightfv(GL_LIGHT1 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
}

void LightSpot::setDirection(glm::vec3 value)
{
	direction = value;
	glLightfv(GL_LIGHT1 + id, GL_SPOT_DIRECTION, glm::value_ptr(direction));
}

void LightSpot::setCutoff(float value)
{
	spot_cutoff = value;
	glLightf(GL_LIGHT1 + id, GL_SPOT_CUTOFF, spot_cutoff);// glm::value_ptr(spot_cutoff));
}

void LightSpot::setExponent(float value)
{
	spot_exponent = value;
	glLightf(GL_LIGHT1 + id, GL_SPOT_EXPONENT, spot_exponent);
}

void LightSpot::enableSpot()
{
	glEnable(GL_LIGHT1 + id);
}

void LightSpot::disableSpot()
{
	glDisable(GL_LIGHT1 + id);
}