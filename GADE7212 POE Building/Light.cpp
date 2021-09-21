#include "Light.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp> // Used to point to values
#include <iostream>

unsigned int Light::currentID = 0; // Specify value for static unsigned int -> treated as a class variable instead of an object

Light::Light()
{
	id = currentID; // Because it is a static variable (shared among all light objects), we can increment it and it will carry thru between lights
	currentID++;
}

void Light::init()
{
	// Initialize the light and tell OpenGL we are going to use the light in the graphics API
	glEnable(GL_LIGHT0 + id);

	glLightfv(GL_LIGHT0 + id, GL_POSITION, glm::value_ptr(position)); // Convert vec4 to a float made of those 4 values
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, glm::value_ptr(ambient));
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, glm::value_ptr(specular));
}

void Light::setPosition(glm::vec4 value)
{
	position = value;
	glLightfv(GL_LIGHT0 + id, GL_POSITION, glm::value_ptr(position));
}

void Light::setAmbient(glm::vec4 value)
{
	ambient = value;
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, glm::value_ptr(ambient));
}

void Light::setDiffuse(glm::vec4 value)
{
	diffuse = value;
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, glm::value_ptr(diffuse));
}

void Light::setSpecular(glm::vec4 value)
{
	specular = value;
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, glm::value_ptr(specular));
}

void Light::enable()
{
	glEnable(GL_LIGHT0 + id);
}

void Light::disable()
{
	glDisable(GL_LIGHT0 + id);
}