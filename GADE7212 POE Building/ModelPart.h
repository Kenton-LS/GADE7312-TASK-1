#pragma once
#include "Vertex.h"
#include <vector>

// Draw multiple model parts as 1 gameObject
// Values are read in from the OBJ file. 
// The list of those values (parameters) are then-after converted into the actual 3d model.
// ModelPart's job is to store that list (so that the verts can be arranged correctly later to draw triangles)

struct ModelPart
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};