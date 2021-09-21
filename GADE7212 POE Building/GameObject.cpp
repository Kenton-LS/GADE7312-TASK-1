#include "GameObject.h"
#include <GL/freeglut.h>

void GameObject::setPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

void GameObject::setPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void GameObject::setIsDrawingOrigin(bool newIsDrawingOrigin)
{
	isDrawingOrigin = newIsDrawingOrigin;
}

void GameObject::setTexture(const char* path)
{
	texture = new Texture(path);
}

void GameObject::draw()
{
	glPushMatrix(); // Ensure the draw happens inside of its own context
	{
		glTranslatef(position.x, position.y, position.z);

		// Texturing START
		if (texture != NULL)
		{
			texture->use();
		}
		// Texturing END

		drawGeometry();
	}
	glPopMatrix();

	/*if (!isDrawingOrigin)
	{
		drawOrigin();
	}*/
}

void GameObject::drawTop()
{
	glPushMatrix(); // Ensure the draw happens inside of its own context
	{
		glTranslatef(position.x, position.y, position.z);

		// Texturing START
		if (texture != NULL)
		{
			texture->use();
		}
		// Texturing END

		drawGeoTop();
	}
	glPopMatrix();

	/*if (!isDrawingOrigin)
	{
		drawOrigin();
	}*/
}

void GameObject::drawGeometry()
{
	glRotatef(45, 1, 1, 0);
	glColor3f(1, 1, 1);
	glutSolidCube(1);
}

void GameObject::drawGeoTop()
{
	glRotatef(45, 1, 1, 0);
	glColor3f(1, 1, 1);
	glutSolidCube(1);
}

void GameObject::drawOrigin()
{
	glDisable(GL_DEPTH_TEST);
	
	glColor3f(1, 0, 0);

	// First thing: draw x axis
	glBegin(GL_LINES);
	{
		glVertex3f(0, 0, 0);
		glVertex3f(originScale, 0, 0); // Used to make the origin bigger and smaller
	}
	glEnd();

	// Push matrix - may need to rotate cone to direction at end of line (for axis) ---> draw cone for x axis
	glPushMatrix();
	{
		glTranslatef(originScale, 0, 0);
		glRotatef(90, 0, 1, 0);
		// (Base radius, height, slices, stack loop cuts in cone)
		glutSolidCone(0.025f, 0.1f, 8, 1);
	}
	glPopMatrix();

	//----------------------------------------------------------------------------------------------------------//

	glColor3f(0, 1, 0);

	// Second thing: draw y axis
	glBegin(GL_LINES);
	{
		glVertex3f(0, 0, 0);
		glVertex3f(0, originScale, 0); // Used to make the origin bigger and smaller
	}
	glEnd();

	// Push matrix - may need to rotate cone to direction at end of line (for axis) ---> draw cone for x axis
	glPushMatrix();
	{
		glTranslatef(0, originScale, 0);
		glRotatef(-90, 1, 0, 0);
		// (Base radius, height, slices, stack loop cuts in cone)
		glutSolidCone(0.025f, 0.1f, 8, 1);
	}
	glPopMatrix();

	//----------------------------------------------------------------------------------------------------------//

	glColor3f(0, 0, 1);

	// Third thing: draw z axis
	glBegin(GL_LINES);
	{
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, originScale); // Used to make the origin bigger and smaller
	}
	glEnd();

	// Push matrix - may need to rotate cone to direction at end of line (for axis) ---> draw cone for x axis
	glPushMatrix();
	{
		glTranslatef(0, 0, originScale);
		glRotatef(0, 1, 1, 0);
		// (Base radius, height, slices, stack loop cuts in cone)
		glutSolidCone(0.025f, 0.1f, 8, 1);
	}
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
}