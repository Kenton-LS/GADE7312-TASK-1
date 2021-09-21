#include <iostream>
#include "Floor.h"
#include <GL/freeglut.h>
using namespace std;

Floor::Floor(vec3 _vStart, float _width, float _height, float _length, vec3 _colour, vec3 _normal)
{
	vStart = _vStart;
	width = _width;
	height = _height;
	length = _length;
	colour = _colour;
	normal = _normal;
}

void Floor::drawGeometry()
{
	// All 4 corners of wall -> calculate values using vectors we have
	// Go clockwise around the wall, bottom leftmost coord = v1
	vec3 v1 = vStart; // Bottom left
	vec3 v2 = v1 - vec3(0, 0, length); // Top left
	vec3 v3 = v2 + vec3(width, 0, 0); // Top right
	vec3 v4 = v1 + vec3(width, 0, 0); // Bottom right


	//vec3 v2 = v1 + vUp; // Top left
	//vec3 v3 = v2 + vLeft; // Top right
	//vec3 v4 = v1 + vLeft; // Bottom right
	cout << "Width: " << width << " Height: " << height << " Length: " << length << endl;

	// Start drawing geometry
	glPushMatrix();
	{
		//glRotatef(180, 1, 0, 0);
		glColor3f(colour.x, colour.y, colour.z);
		glNormal3f(normal.x, normal.y, normal.z); // Specify normals for each face if it were organic...
		// BUT cus it is a plain quad, we just need to make normals face inside the room (cus you only see the room from INSIDE)

		glBegin(GL_QUADS);
		{
			glVertex3f(v1.x, 0, v1.z); // Construct a wall
			glVertex3f(v2.x, 0, v2.z);
			glVertex3f(v3.x, 0, v3.z);
			glVertex3f(v4.x, 0, v4.z);
		}
		glEnd();
	}
	glPopMatrix();
}