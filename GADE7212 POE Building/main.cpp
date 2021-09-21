#include <GL/freeglut.h>
#include <math.h>
#include <cmath>
#include <iostream>

#include "Light.h"
#include "Level.h"

const int WIDTH = 800; // Size of freeglut window
const int HEIGHT = 600;
float twopi = 2.0f * 3.1415926;
float pi = 3.1415926;

float rotationOffset = 0.50f;
float rotationSpeed = 0.50f;

void init(); // For initializing some much needed variables
void display(); // To keep window open for a while
void panDisplay(); // Camera pans around level
void timer(int t);

GameObject gameObject;

Light light1;
Light light2;
Level* level1; // Pointer to a level -> means the object will have to be deleted after use however (check GLUT main)

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GL_RGB | GLUT_DOUBLE | GLUT_DEPTH); // RGB render mode and double buffering, also added depth support for window

	int windowX = (int)(glutGet(GLUT_SCREEN_WIDTH) - WIDTH) / 2;
	int windowY = (int)(glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) / 2;

	glutInitWindowPosition(windowX, windowY); // Glut must create window at this position
	glutInitWindowSize(WIDTH, HEIGHT); // Size of the window to be initialized
	glutCreateWindow("My Very First OpenGL Window"); // Pointer to a char variable (char array / c string)

	//glutDisplayFunc(display); // Show window -> calls the display method periodically
	glutDisplayFunc(panDisplay);

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

		/*gluLookAt(							    // StillCam
			-2.5, 7, 30,						// Zoom in / out (Eye position) 
			-2.5, 7, 0,							// Origin (Point we are looking at)
			0, 1, 0								// (What vector points up)
		);*/
		gluLookAt(							// PanCam
			0, 6, 35,
			0, 6, 0,
			0, 1, 0
		);

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);   // RGB Alpha

	glEnable(GL_LIGHTING); // Enable lightning into the project
	light1.init(); // Intialize first light
	light1.setPosition(glm::vec4(3.0, 2.0, 0.0, 1.0));
	light1.setDiffuse(glm::vec4(0.0, 1.0, 1.0, 1.0));
	light1.setAmbient(glm::vec4(0.0, 1.0, 1.0, 1.0));

	light2.init(); // Intialize second light
	light2.setPosition(glm::vec4(0.0, 2.0, 0.0, 0.0)); // ORIGINAL light2.setPosition(glm::vec4(-3.0, 2.0, 0.0, 1.0));
	light2.setDiffuse(glm::vec4(1.0, 0.0, 1.0, 1.0));
	light2.setAmbient(glm::vec4(1.0, 0.0, 1.0, 1.0));

	level1 = new Level("../Data/level.json"); // SPECIFY STRING PATH TO LEVEL!!!!!!!!
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
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);		// Wait for 60th of a sec, then call timer again
}