#pragma once
#include "GameObject.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

using namespace glm;
using json = nlohmann::json;

class Wall : public GameObject
{
public:
	Wall(vec3 vStart, vec3 vLeft, vec3 vUp, vec3 colour, vec3 normal);

private:
	vec3 vStart; // Center of the room
	vec3 vLeft;
	vec3 vUp;
	vec3 colour;
	vec3 normal;

	void drawGeometry() override;
};