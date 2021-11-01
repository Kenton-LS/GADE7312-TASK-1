#include "CameraController.h"
#include "Time.h"
#include "Input.h" // Static classes: we can declare them anywhere and have access to all their stats / methods
#include "GL/freeglut.h"
#include <iostream>

using namespace glm;

CameraController::CameraController()
{
	// Set variables inside constructor for camera
	forward = vec3(0, 0, -1);
	up = vec3(0, 1, 0);
	right = normalize(cross(forward, up)); // or right = vec3(1, 0, 0) (but more accurate this way)
	position = vec3(); // Creates a vec3(0,0,0) for initial camera position setting
	lookAt = position + forward; // Look at position right in front of us

	updateCamera();
}

void CameraController::updateCamera()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // MUST call before gluLookAt() so that camera transformations apply correctly and work 100%
	gluLookAt( // Call this only after we've figured out the correct coords and vector positions (e.g. vec3 forward)
		position.x, position.y, position.z,
		lookAt.x, lookAt.y, lookAt.z,
		up.x, up.y, up.z
	);
}

void CameraController::update()
{
	deltaTime = Time::getDeltaTime(); // Caching deltaTime so we don't have to keep calling it over and over

	updateLook();
	updatePosition();
	updateCursorLocked();
	updateResetCamera();
	updateCamera();
}

void CameraController::setLookSensitivity(float sensitivity)
{
	lookSensitivity = sensitivity;
}


void CameraController::setMovementSpeed(float speed)
{
	movementSpeed = speed;
}

// THE FINAL RAID BOSS
void CameraController::updateLook()
{
	float xOffset = Input::getMouseDeltaX();
	float yOffset = Input::getMouseDeltaY();

	if (mouseFirstMovement) // For first frame, don't have offset
	{
		xOffset = 0;
		yOffset = 0;
		mouseFirstMovement = false;
	}

	xOffset *= lookSensitivity * deltaTime;
	yOffset *= lookSensitivity * deltaTime;

	yaw += xOffset;
	pitch += yOffset;
	pitch = clamp(pitch, -80.0f, 80.0f); // Clamp pitch so that camera doesn't flip around on us

	float radYaw = radians(yaw);
	float radPitch = radians(pitch);

	forward = normalize(vec3(
		cos(radYaw) * cos(radPitch),
		sin(radPitch),
		sin(radYaw) * cos(radPitch)
	));

	right = normalize(cross(forward, up));
	lookAt = position + forward;

	/*forward = normalize(vec3(
		cos(radians(yaw)) * cos(radians(pitch)),
		sin(radians(pitch)),
		sin(radians(yaw)) * cos(radians(pitch))
	));*/
}

void CameraController::updatePosition()
{
	// forward
	if (Input::getKeyDown('w') || Input::getKeyDown('W')) // Capslock safety check
	{
		position += forward * movementSpeed * deltaTime;
	}
	// backward
	if (Input::getKeyDown('s') || Input::getKeyDown('S'))
	{
		position -= forward * movementSpeed * deltaTime;
	}
	// left
	if (Input::getKeyDown('a') || Input::getKeyDown('A'))
	{
		position -= right * movementSpeed * deltaTime;
	}
	// right
	if (Input::getKeyDown('d') || Input::getKeyDown('D'))
	{
		position += right * movementSpeed * deltaTime;
	}
}

void CameraController::updateCursorLocked()
{
	// Lock mouse cursor to the center of the screen at the right time so it doesnt move out the window
	// E.g. press 1 then cursor locks Press 1 again then cursor unlocks
	// TODO: implement cursor lock

	// cursorLock
	if (Input::getKeyPressed('1'))
	{
		cursorLock = !cursorLock;

	}

	//if (cursorLock == true)
	//{
	//	float centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	//	float centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	//	//std::cout << "X " << centerX << " Y " << centerY << std::endl;

	//	if ((position.x /*+ forward.x*/) != centerX || (position.y /*+ forward.y*/) != centerY)
	//	{
	//		glutWarpPointer(centerX, centerY); // Move pointer to middle of console window
	//	}
	//}

	if (cursorLock == true)
	{
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2); // Move pointer to middle of console window
		lookSensitivity = 0.0f;
	}
	else
	{
		lookSensitivity = 40.0f;
	}
}

void CameraController::updateResetCamera()
{

}

// REFERENCES
// 
// GLUT Warp Pointer
// https://coderedirect.com/questions/390462/how-to-calculate-look-at-point-to-move-the-camera-with-the-mouse-in-opengl-glut