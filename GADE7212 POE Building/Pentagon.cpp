#include "Pentagon.h"
#include <cmath>
#include <GL/freeglut.h>

void Pentagon::drawGeometry() // Rotate the shape
{
	glPushMatrix();
	{
		glRotatef(rotationOffset, 1, 1, 1);
		buildGeometry();
	}
	glPopMatrix();
	rotationOffset += rotationSpeed;
};

void Pentagon::buildGeometry() // Forge the actual shape
{
	glPushMatrix();

	//glTranslatef(WIDTH / 2, HEIGHT / 2, 0); // Move to this position, start drawing from here
	glRotatef(rotationOffset, 0, 1, 0);

	glBegin(GL_TRIANGLE_FAN);
	{										// Add vertices in here
		glColor3f(0, 1, 1);
		glVertex2f(0, 0);					// Midpoint -> uses width and height constants

		glColor3f(1, 0, 1);
		for (int j = 0; j <= 8; j++)
		{
			glVertex2f(
				/*WIDTH / 2 + */cos(j * pi / 4.0) * size,
				/*HEIGHT / 2 + */sin(j * pi / 4.0) * size
			);
		}
		glEnd();
	}
	glPopMatrix();
	rotationOffset += rotationSpeed;
};