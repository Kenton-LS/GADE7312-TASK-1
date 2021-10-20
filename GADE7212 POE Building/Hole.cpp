#include "Hole.h"
#include <GL/freeglut.h>

Hole::Hole(vec3 _vStart, vec3 _vLeft, vec3 _vUp, bool _southFace, float _offset, float _holeWidth, float _holeHeight, float _roomPadding, vec3 _colour, vec3 _normal) // Underscore for assigning variables
{
	vStart = _vStart;
	vLeft = _vLeft;
	vUp = _vUp;
	southFace = _southFace; // True = hole is on south face, false = east face
	offset = _offset; // Of hole from bottom-most left corner of the wall (AKA vStart)
	holeWidth = _holeWidth; // How big the hole is
	holeHeight = _holeHeight; // NEW FOR TASK 2: making that 'slab' above each doorway
	roomPadding = _roomPadding;
	colour = _colour;
	normal = _normal;
}

void Hole::drawGeometry()
{
	vec3 v1, v2, v3, v4;
	vec3 vh1, vh2, vh3, vh4; 
	vec3 vs1, vs2, vs3, vs4; // For SLAB (Task 2)

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

		// For SLAB (Task 2)
		vs1 = v2 + vec3(offset - roomPadding, 0, 0) - vec3(0, vUp.y - holeHeight, 0); // Hole's roof slab -> bottom left
		vs2 = v2 + vec3(offset - roomPadding, 0, 0); // Hole's roof slab -> top left
		vs3 = vs2 + vec3(holeWidth, 0, 0); // Hole's roof slab -> top right
		vs4 = vs3 - vec3(0, vUp.y - holeHeight, 0); // Hole's roof slab -> bottom right
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

		// For SLAB (Task 2)
		vs1 = v2 - vec3(0, 0, offset - roomPadding) - vec3(0, vUp.y - holeHeight, 0); // Hole's roof slab -> bottom left
		vs2 = v2 - vec3(0, 0, offset - roomPadding); // Hole's roof slab -> top left
		vs3 = vs2 - vec3(0, 0, holeWidth); // Hole's roof slab -> top right
		vs4 = vs3 - vec3(0, vUp.y - holeHeight, 0); // Hole's roof slab -> bottom right
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

				// Slab above hole
				glTexCoord2f(0, 0);
				glVertex3f(vs1.x, vs1.y, vs1.z); // VS1

				glTexCoord2f(0, glm::length(vUp));
				glVertex3f(vs2.x, vs2.y, vs2.z); // VS2

				glTexCoord2f(glm::length(offset), glm::length(vUp));
				glVertex3f(vs3.x, vs3.y, vs3.z); // VS3

				glTexCoord2f(glm::length(offset), 0);
				glVertex3f(vs4.x, vs4.y, vs4.z); // VS4
				// End slab

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

				// Slab above hole
				glTexCoord2f(0, 0);
				glVertex3f(vs1.x, vs1.y, vs1.z); // VS1

				glTexCoord2f(0, glm::length(vUp));
				glVertex3f(vs2.x, vs2.y, vs2.z); // VS2

				glTexCoord2f(glm::length(offset), glm::length(vUp));
				glVertex3f(vs3.x, vs3.y, vs3.z); // VS3

				glTexCoord2f(glm::length(offset), 0);
				glVertex3f(vs4.x, vs4.y, vs4.z); // VS4
				// End slab

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