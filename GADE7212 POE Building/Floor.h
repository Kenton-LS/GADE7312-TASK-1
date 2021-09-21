#pragma once
#include "GameObject.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

using namespace glm;
using json = nlohmann::json;

class Floor : public GameObject
{
public:
	Floor(vec3 floorPoints[], vec3 colour, vec3 normal);

private:
	vec3 floorPoints[4]; // Center of the room, with y = 0 for placing floor
	vec3 colour;
	vec3 normal;

	void drawGeometry() override;
};

