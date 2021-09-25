#pragma once
#include "GameObject.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

using namespace glm;
using json = nlohmann::json;

class Hole : public GameObject
{
public:
	Hole(vec3 vStart, vec3 vLeft, vec3 vUp, bool southFace, float offset, float holeWidth, float roomPadding, vec3 colour, vec3 normal);

private:
	vec3 vStart; // Bottom left corner of wall
	vec3 vLeft;
	vec3 vUp;
	bool southFace;
	float offset; // Of hole from bottom-most left corner of the wall (AKA vStart)
	float holeWidth; // How big the hole is
	float roomPadding;
	vec3 colour;
	vec3 normal;

	void drawGeometry() override;
};