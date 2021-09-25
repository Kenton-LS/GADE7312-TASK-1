#pragma once
#include "GameObject.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

using namespace glm;
using json = nlohmann::json;

class Roof : public GameObject
{
public:
	Roof(vec3 vStart, float width, float height, float length, vec3 colour, vec3 normal);

private:
	vec3 vStart; // Center of the room
	float width;
	float height;
	float length;
	vec3 colour;
	vec3 normal;

	void drawGeometry() override;
};