#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp> // Used to point to values
#include <iostream>

#include "LightPoint.h"

unsigned int LightPoint::PcurrentID = 0;

LightPoint::LightPoint() {
	id = PcurrentID; // Because it is a static variable (shared among all light objects), we can increment it and it will carry thru between lights
	PcurrentID++;
}

void LightPoint::init()
{
	// Initialize the light and tell OpenGL we are going to use the light in the graphics API
	glEnable(GL_LIGHT2 + id);

	glLightfv(GL_LIGHT2 + id, GL_POSITION, glm::value_ptr(position)); // Convert vec4 to a float made of those 4 values
	glLightfv(GL_LIGHT2 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
	glLightfv(GL_LIGHT2 + id, GL_AMBIENT, glm::value_ptr(ambient));
}

void LightPoint::setPosition(glm::vec4 value)
{
	position = value;
	glLightfv(GL_LIGHT2 + id, GL_POSITION, glm::value_ptr(position));
}
void LightPoint::setAmbient(glm::vec4 value)
{
	ambient = value;
	glLightfv(GL_LIGHT2 + id, GL_AMBIENT, glm::value_ptr(ambient));
}

void LightPoint::setDiffuse(glm::vec4 value)
{
	diffuse = value;
	glLightfv(GL_LIGHT2 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
}

void LightPoint::setLAttenuation(float value) {
	linearA = value;
	glLightf(GL_LIGHT2 + id, GL_LINEAR_ATTENUATION, linearA);
}

void LightPoint::setCAttenuation(float value) {
	constantA = value;
	glLightf(GL_LIGHT2 + id, GL_CONSTANT_ATTENUATION, constantA);
}

void LightPoint::setQAttenuation(float value) {
	quadraticA = value;
	glLightf(GL_LIGHT2 + id, GL_QUADRATIC_ATTENUATION, quadraticA); //lower == brighter
}

void LightPoint::enablePoint()
{
	glEnable(GL_LIGHT2 + id);
}

void LightPoint::disablePoint()
{
	glDisable(GL_LIGHT2 + id);
}