#pragma once
#include "Texture.h" 
#include <glm/glm.hpp>

#define PI = 3.14159265

class GameObject
{
private:
	glm::vec3 position;
	float originScale = 0.5f;
	bool isDrawingOrigin = false;
	Texture* texture;

	void drawOrigin();
	virtual void drawGeometry();
	virtual void drawGeoTop();

public:
	void draw();
	void drawTop();
	void setPosition(float x, float y, float z); // Can specify object coords with either xyz separately, or vector 3 coords
	void setPosition(glm::vec3 newPosition);
	void setIsDrawingOrigin(bool newIsDrawingOrigin);
	void setTexture(const char* path);
};