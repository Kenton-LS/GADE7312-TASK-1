#include "Wall.h"
#include <GL/freeglut.h>

Wall::Wall(vec3 _vStart, vec3 _vLeft, vec3 _vUp, vec3 _colour, vec3 _normal) // Underscore for assigning variables
{
	vStart = _vStart;
	vLeft = _vLeft;
	vUp = _vUp;
	colour = _colour;
	normal = _normal; // NOTE FOR THE FUTURE: WALL WILL HAVE TO MANAGE MAKING DOORS TOO
}

void Wall::drawGeometry()
{
	// All 4 corners of wall -> calculate values using vectors we have
	// Go clockwise around the wall, bottom leftmost coord = v1
	vec3 v1 = vStart; // Bottom left
	vec3 v2 = v1 + vUp; // Top left
	vec3 v3 = v2 + vLeft; // Top right
	vec3 v4 = v1 + vLeft; // Bottom right

	// Start drawing geometry
	glPushMatrix();
	{
		glColor3f(colour.x, colour.y, colour.z);
		glNormal3f(normal.x, normal.y, normal.z); // Specify normals for each face if it were organic...
		// BUT cus it is a plain quad, we just need to make normals face inside the room (cus you only see the room from INSIDE)

		glBegin(GL_QUADS);
		{
			glVertex3f(v1.x, v1.y, v1.z); // Construct a wall
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v3.x, v3.y, v3.z);
			glVertex3f(v4.x, v4.y, v4.z);
		}
		glEnd();
	}
	glPopMatrix();
}