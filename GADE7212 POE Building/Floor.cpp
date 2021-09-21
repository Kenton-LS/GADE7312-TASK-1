#include "Floor.h"
#include <GL/freeglut.h>

Floor::Floor(vec3 _floorPoints[], vec3 _colour, vec3 _normal) // Underscore for assigning variables
{
	floorPoints[4] = _floorPoints[4];
	colour = _colour;
	normal = _normal; 
}

void Floor::drawGeometry()
{
	// All 4 corners of floor -> calculate values using vectors we have!
	// Go clockwise around the floor, bottom leftmost coord = v1
	vec3 v1 = floorPoints[0]; // Bottom left
	vec3 v2 = floorPoints[1]; // Top left
	vec3 v3 = floorPoints[2]; // Top right
	vec3 v4 = floorPoints[3]; // Bottom right

	// Start drawing geometry
	glPushMatrix();
	{
		glColor3f(0, 0, 1);
		glNormal3f(normal.x, normal.y, normal.z); // Specify normals for each face if it were organic...
		// BUT cus it is a plain quad, we just need to make normals face inside the room (cus you only see the room from INSIDE)

		glBegin(GL_QUADS);
		{
			glVertex3f(v1.x, 0, v1.z); // Construct a floor
			glVertex3f(v2.x, 0, v2.z);
			glVertex3f(v3.x, 0, v3.z);
			glVertex3f(v4.x, 0, v4.z);
		}
		glEnd();
	}
	glPopMatrix();
}