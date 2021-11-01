#include "Input.h"
#include <GL/freeglut.h>
#include <string>
#include <sstream> // Could help in making a console for the game (hint hint)
#include <iostream>

using namespace std;

// Static variables... HAVE TO BE INITIALIZED IN CLASS FILE

int Input::prevMouseX;
int Input::prevMouseY;
int Input::mouseX;
int Input::mouseY;
int Input::mouseXCached;
int Input::mouseYCached;

map<unsigned char, int> Input::keysDown;
map<unsigned char, int> Input::keysPressed;
map<unsigned char, int> Input::keysReleased;

void Input::mousePositionUpdate(int x, int y)
{
	//cout << x << ", " << y << endl;
	mouseXCached = x; // So that we can update the actual and previous mouse X's on each frame (for correct mouse position)
	mouseYCached = y; // Cached variable is basically a temp variable for updating the actual variables
}

void Input::keyboardUpdate(unsigned char key, int mouseX, int mouseY) // If we get a keyboard update with a key, it means that key is being held down
{
	// Callback function telling us that a key is being held down

	//cout << "Key down: " << key << endl;

	// (1) Register that it was pressed
	// (2) Register that it is down (via an increasing integer)

	if (keysDown.find(key) == keysDown.end()) // If key is not in the keysdown map
	{
		keysDown[key] = 0; // Add it to the map - > 0 represents the start of the button being pressed
	}

	if (keysDown[key] == 0)
	{
		keysDown[key] = 1; // An interaction is actually occurring
		keysPressed[key] = 1;
	}
	else if (keysDown[key] > 0)
	{
		// Just increment it
		keysDown[key]++; // Allows us to know HOW MANY FRAMES a key has been HELD DOWN FOR
	}
}

void Input::keyboardUpUpdate(unsigned char key, int mouseX, int mouseY) // When the key is actually released
{
	//cout << "Key released: " << key << endl;

	keysReleased[key] = 1; // This key has now been released
	keysDown[key] = 0; // 0 because key is NO LONGER BEING PRESSED
}

void Input::updateBefore() // Called at the beginning of the Update Display Loop (ensure that pressed keys only register for a single frame if quick tapped)
{
	map<unsigned char, int>::iterator it; // Create an iterator to run through / loop over our map (has to be done in C++)

	// Update mouse position
	prevMouseX = mouseX;
	prevMouseY = mouseY;
	mouseX = mouseXCached;
	mouseY = mouseYCached;

	for (it = keysPressed.begin(); it != keysPressed.end(); ++it) // As long as iterator is not equal to the end of the dictionary, keep plussing it / looping through
	{
		if (keysPressed[it->first] == 0) // [it->first] is the key in the <key-value> pair. [it->second] would be the value in the <key-value> pair.
		{
			continue;
		}
		keysPressed[it->first]++; // If not 0, increase it by 1
	}

	for (it = keysReleased.begin(); it != keysReleased.end(); ++it)
	{
		if (keysReleased[it->first] == 0) // Can also say if (it->second == 0), gives same effect
		{
			continue;
		}
		keysReleased[it->first]++;
	}

	// Now check if any key has been active for longer than a frame. If not, set it back to 0.
}

void Input::updateAfter() // Called at the very end of the Update Display Loop
{
	outputKeysMap(keysPressed, "Keys Pressed: ");
	outputKeysMap(keysDown, "Keys Down: ");
	outputKeysMap(keysReleased, "Keys Released: ");

	// Iterate through all released keys, if value is bigger than 1 then it is no longer being used.
	map<unsigned char, int>::iterator it;

	for (it = keysReleased.begin(); it != keysReleased.end(); ++it)
	{
		if (it->second > 1) // Key released for more than a frame
		{
			keysReleased[it->first] = 0;
		}
	}

	for (it = keysPressed.begin(); it != keysPressed.end(); ++it)
	{
		if (it->second > 1) // Key pressed for more than a frame
		{
			keysPressed[it->first] = 0;
		}
	}
}

void Input::outputKeysMap(map<unsigned char, int> keysMap, string info)
{
	map<unsigned char, int>::iterator it; // Create iterator

	stringstream stream;
	string output;

	// Code here is not necessary, but serves extremely helpful in debugging
	// Loop over all the keys that could possibly be pressed at this stage -> BUILD A STRING TO SHOW ALL THE KEYS BEING PRESSED
	for (it = keysMap.begin(); it != keysMap.end(); ++it)
	{
		if (it->second > 0)
		{
			stream << it->first; // We created a stream, built the stream, and wrote it back into a string variable... (because cannot add char to a string in C++)
		}
	}
	stream >> output; // Puts it back into the string we created

	if (!output.empty()) // If it is not empty, print all to console
	{
		//cout << info << output << endl;
	}
}

int  Input::getMouseX()
{
	return mouseX;
}

int  Input::getMouseY()
{
	return mouseY;
}

int  Input::getMouseDeltaX(bool inverted)
{
	if (inverted)
	{
		return prevMouseX - mouseX;
	}
	return mouseX - prevMouseX;
}

int  Input::getMouseDeltaY(bool inverted)
{
	if (inverted)
	{
		return mouseY - prevMouseY;
	}
	return prevMouseY - mouseY;
}

bool Input::getKeyDown(unsigned char key)
{
	return keysDown[key] > 0;
}

bool Input::getKeyPressed(unsigned char key)
{
	return keysPressed[key] > 0;
}

bool Input::getKeyReleased(unsigned char key)
{
	return keysReleased[key] > 0;
}