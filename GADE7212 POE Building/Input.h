#pragma once
#include <map> // AKA Hashmap
#include <string> // For debugging method

static class Input // Static because they are just for grabbing info from, not for creating objects of
{
private:
	// Look at GLUT when mouse is manipulated / pressed
	static int prevMouseX; // Integers because based on pixels (if float it would be more accurate, but for now this is fine)
	static int prevMouseY;
	static int mouseX; // Current mouse position
	static int mouseY;
	static int mouseXCached; // Keeps latest X value that will be triggered. When we use our self-made UPDATE fucntion, we'll assign this stored vlue to the proper mouseX
	static int mouseYCached; // Thus, allows mouse deltas and positions to be calculated more accurately

	// 3 Hashmaps -> 1 for keypress on single frame, 
	// 1 for key pressed down for continuous frames, (how long key held down)
	// 1 for when key is released

	static std::map<unsigned char, int> keysDown; // WHEN A KEY IS BEING HELD DOWN (it is stored in this map - > even for continuous amounts of frames if necessary)
	static std::map<unsigned char, int> keysPressed; // THE FIRST FRAME THAT THE KEY WAS PRESSED (only for that 1st frame the key was pressed)
	static std::map<unsigned char, int> keysReleased; // THE FRAME THAT THE KEY WAS RELEASED
public:
	static void mousePositionUpdate(int x, int y); // Callback method of the GLUT event - > Tells us WHENEVER THE MOUSE HAS BEEN MOVED
	static void keyboardUpdate(unsigned char key, int mouseX, int mouseY); // Tells us IF THIS KEY IS DOWN (key that was pressed, mouseX, mouseY)
	static void keyboardUpUpdate(unsigned char key, int mouseX, int mouseY); // Tells us IF THIS KEY IS RELEASED

	// The 2 methods below: for managing when certain keys should be removed - > CALLED AT THE BEGIN AND END OF EACH FRAME to help registering button presses better 

	static void updateBefore();
	static void updateAfter();

	static int getMouseX();
	static int getMouseY();
	static int getMouseDeltaX(bool inverted = false); // Optional invert axis
	static int getMouseDeltaY(bool inverted = false);

	static bool getKeyDown(unsigned char key); // Parse in char
	static bool getKeyPressed(unsigned char key); // Returns true if <<char entered>> was pressed
	static bool getKeyReleased(unsigned char key);

	static void outputKeysMap(std::map<unsigned char, int> keysMap, std::string info); // New addition, merely for debugging 
};