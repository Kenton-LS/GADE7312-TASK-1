#pragma once
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

class CameraController
{
public:
	CameraController();
	void update();

	void setMovementSpeed(float speed);
	void setLookSensitivity(float sensitivity);
private:
	float deltaTime; // Just for caching deltaTime

	float movementSpeed = 15.0f; // 5.0f // Default
	float lookSensitivity = 40.0f;

	bool mouseFirstMovement = true;

	float yaw = -90.0f; // Frisbee (throwing a discus)
	float pitch = 0; // Bowling (in cricket)

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 lookAt; // Direction we are looking in

	bool cursorLock = false;

	void updateLook();
	void updatePosition();
	void updateCursorLocked();
	void updateResetCamera();
	void updateCamera();
};