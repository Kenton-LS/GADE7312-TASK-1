#include "LightSun.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp> // Used to point to values
#include <iostream>

unsigned int LightSun::currentID = 0; // Specify value for static unsigned int -> treated as a class variable instead of an object

LightSun::LightSun()
{
	id = currentID; // Because it is a static variable (shared among all light objects), we can increment it and it will carry thru between lights
	currentID++;
}

void LightSun::init()
{
	// Initialize the light and tell OpenGL we are going to use the light in the graphics API
	glEnable(GL_LIGHT0 + id);

	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, glm::value_ptr(ambient));
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, glm::value_ptr(specular));
}

void LightSun::setAmbient(glm::vec4 value)
{
	ambient = value;
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, glm::value_ptr(ambient));
}

void LightSun::setDiffuse(glm::vec4 value)
{
	diffuse = value;
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
}

void LightSun::setSpecular(glm::vec4 value)
{
	specular = value;
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, glm::value_ptr(specular));
}

void LightSun::enableSun()
{
	glEnable(GL_LIGHT0 + id);
}

void LightSun::disableSun()
{
	glDisable(GL_LIGHT0 + id);
}