#include "Time.h"
#include <GL/freeglut.h>

int Time::prevTime = 0;
int Time::time = 0;

void Time::update()
{
	prevTime = time;
	time = glutGet(GLUT_ELAPSED_TIME); // Time since beginning of the program - > integer that displays MILLISECONDS
}

float Time::getTime() // Convert time from milli to secs
{
	return time / 1000.0f;
}

float Time::getDeltaTime() // Difference between current and previous frames - > time between 2 frames in SECONDS
{
	return (time - prevTime) / 1000.0f;
}

float Time::getFrameRate() // Number of frames per second
{
	return 1.0f / getDeltaTime(); // 1.0f or 1 represents a SECOND - > number of deltaTimes every second
}