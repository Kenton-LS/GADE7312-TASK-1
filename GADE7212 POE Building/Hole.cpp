#include "Hole.h"
#include <GL/freeglut.h>

Hole::Hole(vec3 _vStart, vec3 _vLeft, vec3 _vUp, bool _southFace, float _offset, float _holeWidth, float _roomPadding, vec3 _colour, vec3 _normal) // Underscore for assigning variables
{
	vStart = _vStart;
	vLeft = _vLeft;
	vUp = _vUp;
	southFace = _southFace; // True = hole is on south face, false = east face
	offset = _offset; // Of hole from bottom-most left corner of the wall (AKA vStart)
	holeWidth = _holeWidth; // How big the hole is
	roomPadding = _roomPadding;
	colour = _colour;
	normal = _normal;
}

void Hole::drawGeometry()
{
	vec3 v1, v2, v3, v4;
	vec3 vh1, vh2, vh3, vh4; 

	if (southFace == true)
	{
		// Wall verts
		v1 = vStart; // Bottom left
		v2 = v1 + vUp; // Top left
		v3 = v2 + vLeft; // Top right
		v4 = v1 + vLeft; // Bottom right

		// Hole verts (situated inside the wall verts to form a gap -> [ [] ]  )
		vh1 = v1 + vec3(offset - roomPadding, 0, 0); // Bottom Left
		vh2 = v2 + vec3(offset - roomPadding, 0, 0); // Top left
		vh3 = vh2 + vec3(holeWidth, 0, 0); // Top right
		vh4 = vh1 + vec3(holeWidth, 0, 0); // Bototm right
	}
	else
	{
		// Wall verts
		v1 = vStart; // Bottom left
		v2 = v1 + vUp; // Top left
		v3 = v2 + vLeft; // Top right
		v4 = v1 + vLeft; // Bottom right

		// Hole verts (situated inside the wall verts to form a gap -> [ [] ]  )
		vh1 = v1 - vec3(0, 0, offset - roomPadding); // Bottom Left
		vh2 = v2 - vec3(0, 0, offset - roomPadding); // Top left
		vh3 = vh2 - vec3(0, 0, holeWidth); // Top right
		vh4 = vh1 - vec3(0, 0, holeWidth); // Bottom right
	}

	// Start drawing geometry
	glPushMatrix();
	{
		glColor3f(colour.x, colour.y, colour.z);
		glNormal3f(normal.x, normal.y, normal.z);

		glBegin(GL_QUADS); // Construct a normal wall, but with added hole verts
		{  
			if (southFace == true) // SOUTH FACE WALL
			{
				// Left side wall
				glTexCoord2f(0, 0);
				glVertex3f(v1.x, v1.y, v1.z); // V1

				glTexCoord2f(0, glm::length(vUp));
				glVertex3f(v2.x, v2.y, v2.z); // V2

				// Hole in middle
				glTexCoord2f(glm::length(offset), glm::length(vUp));
				glVertex3f(vh2.x, vh2.y, vh2.z); // VH2

				glTexCoord2f(glm::length(offset), 0);
				glVertex3f(vh1.x, vh1.y, vh1.z); // VH1

				glTexCoord2f(0, 0);
				glVertex3f(vh4.x, vh4.y, vh4.z); // VH4

				glTexCoord2f(0, glm::length(vUp));
				glVertex3f(vh3.x, vh3.y, vh3.z); // VH3

				// Right side wall
				glTexCoord2f(glm::length(offset), glm::length(vUp));
				glVertex3f(v3.x, v3.y, v3.z); // V3

				glTexCoord2f(glm::length(offset), 0);
				glVertex3f(v4.x, v4.y, v4.z); //V4
			}
			else // EAST FACE WALL
			{
				// Left side wall
				glTexCoord2f(0, 0);
				glVertex3f(v1.x, v1.y, v1.z); // V1

				glTexCoord2f(0, glm::length(vUp));
				glVertex3f(v2.x, v2.y, v2.z); // V2

				// Hole in middle
				glTexCoord2f(glm::length(offset), glm::length(vUp));
				glVertex3f(vh2.x, vh2.y, vh2.z); // VH2

				glTexCoord2f(glm::length(offset), 0);
				glVertex3f(vh1.x, vh1.y, vh1.z); // VH1

				glTexCoord2f(0, 0);
				glVertex3f(vh4.x, vh4.y, vh4.z); // VH4

				glTexCoord2f(0, glm::length(vUp));
				glVertex3f(vh3.x, vh3.y, vh3.z); // VH3

				// Right side wall
				glTexCoord2f(glm::length(offset), glm::length(vUp));
				glVertex3f(v3.x, v3.y, v3.z); // V3

				glTexCoord2f(glm::length(offset), 0);
				glVertex3f(v4.x, v4.y, v4.z); //V4
			}
		}
		glEnd();
	}
	glPopMatrix();
}