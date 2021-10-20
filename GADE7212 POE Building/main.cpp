#include <GL/freeglut.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <time.h>

#include "Light.h"
#include "Level.h" // Calls Room.h
#include "Loader.h" // Calls Model.h
#include "LightSun.h"
#include "LightSpot.h"
#include "LightPoint.h"

const int WIDTH = 1000; // Size of freeglut window
const int HEIGHT = 800;
float twopi = 2.0f * 3.1415926;
float pi = 3.1415926;

float rotationOffset = 0.50f;
float rotationSpeed = 0.50f;

// FPS Variables
int initial_time = time(NULL); // Initialized at start
int final_time;
int frame_count = 0; // Initially 0

void init(); // For initializing some much needed variables
void display(); // To keep window open for a while
void panDisplay(); // Camera pans around level
void timer(int t);
void calculateFPS();

GameObject gameObject;

Light light1;
Light light2;
Level* level1; // Pointer to a level -> means the object will have to be deleted after use however (check GLUT main)

LightSun lightSun;
LightSpot lightSpot;
LightPoint lightPoint;

//------CONTROL HUB------//
bool hasPan = true;
bool hasRoof = true;
bool hasFloor = true;
bool hasDoors = true;
bool hasLight = true;
bool night = true;
// If hasLight = false, make night = false
// If night = true, make hasPan and hasRoof = false
//-----------------------//

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GL_RGB | GLUT_DOUBLE | GLUT_DEPTH); // RGB render mode and double buffering, also added depth support for window

	int windowX = (int)(glutGet(GLUT_SCREEN_WIDTH) - WIDTH) / 2;
	int windowY = (int)(glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) / 2;

	glutInitWindowPosition(windowX, windowY); // Glut must create window at this position
	glutInitWindowSize(WIDTH, HEIGHT); // Size of the window to be initialized
	glutCreateWindow("My Very First OpenGL Window"); // Pointer to a char variable (char array / c string)

	if (hasPan)
	{
		glutDisplayFunc(panDisplay);
	}
	else
	{
		glutDisplayFunc(display); // Show window -> calls the display method periodically
	}

	glutTimerFunc(0, timer, 0); // Used to time frames

	init(); // change window color (see Init())
	glutMainLoop(); // Infinite loop window display until exit button clicked

	delete level1; // DELETE LEVEL HERE!!!

	return 0;
}

void init()
{
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);			// Set colour of window (avoid white or black)
	glLoadIdentity();						// Reset translation and rotation to 0

											//glOrtho(0, WIDTH, HEIGHT, 0, -100, 100); // Isometric camera, no depth 
	gluPerspective(50.0, (double) WIDTH / (double) HEIGHT, 1.0, 1000.0); // Perspective camera, has depth

											//(Bottom, Width, Height, Top, Near, Far) -> near & far = minecraft render distance
	glMatrixMode(GL_MODELVIEW);

											// Where camera sits, where center looks at, specify which vector is up (Y in this case)
											// First 3: origin, next 3: center (0,0,0), next 3: the vector pointing up

	if (hasPan)
	{
		gluLookAt(							// PanCam
			0, 6, 35,
			0, 6, 0,
			0, 1, 0
		);
	}
	else
	{
		gluLookAt(							    // StillCam
			-2.5, 7, 30,						// Zoom in / out (Eye position) 
			-2.5, 7, 0,							// Origin (Point we are looking at)
			0, 1, 0								// (What vector points up)
		);
	}

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);   // RGB Alpha

	if (hasLight)
	{
		glEnable(GL_LIGHTING); // Enable lightning into the project

		lightSun.init(); // Intialize first light

		if (night)
		{
			lightSun.setDiffuse(glm::vec4(0.2, 0.2, 0.2, 0.2));
			lightSun.setAmbient(glm::vec4(0.2, 0.2, 0.2, 0.2));
		}
		else
		{
			lightSun.setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0));
			lightSun.setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0));
		}
		lightSun.setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0));

		lightSpot.init(); // Intialize second light

		lightSpot.setPosition(glm::vec4(-3.0, 0.0, 3.0, 1.0));
		lightSpot.setDiffuse(glm::vec4(0.0, 1.0, 0.0, 1.0));
		lightSpot.setAmbient(glm::vec4(0.0, 1.0, 0.0, 1.0));

		lightSpot.setCutoff(55.0);
		lightSpot.setDirection(glm::vec3(1.0, 2.0, 1.5));
		lightSpot.setExponent(1.0);

		lightPoint.init(); // Intialize third light

		lightPoint.setPosition(glm::vec4(0.0, 1.0, 0.0, 1.0));
		lightPoint.setDiffuse(glm::vec4(1.0, 0.0, 0.0, 1.0));
		lightPoint.setAmbient(glm::vec4(1.0, 0.0, 0.0, 1.0));

		lightPoint.setCAttenuation(1.0);//stays 1 for constant emmission
		lightPoint.setLAttenuation(0.1);
		lightPoint.setQAttenuation(0.06);
	}
	else
	{
		glEnable(GL_LIGHTING); // Enable lightning into the project
		light1.init(); // Intialize first light
		light1.setPosition(glm::vec4(3.0, 2.0, 0.0, 1.0));
		light1.setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0));
		light1.setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0));

		light2.init(); // Intialize second light
		light2.setPosition(glm::vec4(0.0, 2.0, 0.0, 0.0)); // ORIGINAL light2.setPosition(glm::vec4(-3.0, 2.0, 0.0, 1.0));
		light2.setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0));
		light2.setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0));
	}

	level1 = new Level("../Data/level.json", hasRoof, hasFloor, hasDoors); // SPECIFY STRING PATH TO LEVEL!!!!!!!!
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Setup depth testing

	glPushMatrix(); // Load Level
	{
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0); // 90 degrees for bird's eye view

		level1->draw(); // LOAD THE LEVEL 
	}
	glPopMatrix();
	glutSwapBuffers();						// Swap frames and draw on the new screen frame

	calculateFPS();
}

void panDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	{
		glRotatef(rotationOffset, 0, 1, 0);
		glTranslatef(0, 0, 0);
		glRotatef(60, 1, 0, 0);

		level1->draw();
	}
	glPopMatrix();
	rotationOffset += rotationSpeed;
	glutSwapBuffers();

	calculateFPS();
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);		// Wait for 60th of a sec, then call timer again
}

void calculateFPS()
{
	// For FPS
	frame_count++;
	final_time = time(NULL); // Returns seconds
	// If the time between initial & current time is > 1, a second has passed
	if (final_time - initial_time > 0)
	{
		// Calculate Frame Rate
		// Formula: frames drawn / time taken (in seconds) = fps
		std::cout << "FPS: " << frame_count / (final_time - initial_time) << std::endl;
		frame_count = 0; // Reset for next time we calculate (for every sec that passes)
		initial_time = final_time;
	}
}

// REFERENCES
// FPS counter
// https://www.youtube.com/watch?v=CYDFQ1oJJdI