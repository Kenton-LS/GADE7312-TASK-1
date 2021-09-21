#pragma once
#include "GameObject.h"

class Pentagon : public GameObject
{
public:
	float rotationOffset = 1.0f;
	float rotationSpeed = 1.0f;
private:
	virtual void drawGeometry() override;
	virtual void buildGeometry();
};