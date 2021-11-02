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

#include "ModelWhole.h" // TASK 2 Loading in models
#include "CameraController.h" // TASK 2 Camera movement
#include "Input.h" // TASK 2 Tracking mouse coords
#include "Time.h" // TASK 2 Tracking Frames Per Second & updating cam position

// STB TEXTURES
#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

// Tiny OBJ loader
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

const int WIDTH = 1000; // Size of freeglut window
const int HEIGHT = 800;
float rotationOffset = 0.50f;
float rotationSpeed = 0.50f;

// FPS Variables
//int initial_time = time(NULL); // Initialized at start
//int final_time;
//int frame_count = 0; // Initially 0
//void calculateFPS();

void init(); // For initializing some much needed variables
void controlDisplay(); // TASK 2 Controllable cam
void stillDisplay(); // To keep window open for a while
void panDisplay(); // Camera pans around level
void setupLighting(); // Manages types of lights to spawn
void timer(int t);

void commandManager(); // Holds functionality for activating commands (aka command update function)
void commandCheck(); // Real-time game engine affector: controls model spawning, level loading, et cetera
void commandGiver(); // Reminds user what commands they can input
void drawModels(); // Draws all models in the scene (including new console-created models)
void drawLevels(); // Checks & draws whichever level is currently active
void calculateTriangleCount(); // For calculating total triangles (num of vertices divided by 3)

bool spamFPS = false;

GameObject gameObject;
Light light1;
Light light2;

LightSun lightSun;
LightSpot lightSpot;
LightPoint lightPoint;

//------TASK 2------//
CameraController* camera; // TASK 2 pointer to freeroam cam

std::vector<ModelWhole*> models;
ModelWhole* model1; // TASK 2 pointers to models array

Level* level1; // Pointer to a level -> means the object will have to be deleted after use however (check GLUT main)
Level* level2;
//------------------//

//------CONTROL HUB------//
int camType = 3;				// 1 = PAN (rotate around level) ||| 2 = STILL (bird's eye view) ||| 3 = CONTROL (free roam via user input)
int activeLevel = 1;			// 1 or 2
bool hasRoof = true;
bool hasFloor = true;
bool hasDoors = true;
bool hasLight = false;			// If hasLight = false, make night = false
bool night = false;				// If night = true, make hasPan and hasRoof = false
//-----------------------//

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GL_RGB | GLUT_DOUBLE | GLUT_DEPTH); // RGB render mode and double buffering, also added depth support for window

	int windowX = (int)(glutGet(GLUT_SCREEN_WIDTH) - WIDTH) / 2;
	int windowY = (int)(glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) / 2;

	glutInitWindowPosition(windowX, windowY); // Glut must create window at this position
	glutInitWindowSize(WIDTH, HEIGHT); // Size of the window to be initialized
	glutCreateWindow("FrameWearer Engine"); // Pointer to a char variable (char array / c string)

	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2); // Move pointer to middle of console window

	switch (camType) // 1 = PAN (rotate around level) ||| 2 = STILL (bird's eye view) ||| 3 = CONTROL (free roam via user input)
	{
		default:
		case 1:
			glutDisplayFunc(panDisplay);
			break;
		case 2:
			glutDisplayFunc(stillDisplay);
			break;
		case 3:
			glutDisplayFunc(controlDisplay); // Show window -> calls the display method periodically
			break;
	}

	glutTimerFunc(0, timer, 0); // Used to time frames

	init(); // change window color (see Init())
	glutMainLoop(); // Infinite loop window display until exit button clicked

	delete level1; // DELETE LEVEL HERE!!!
	delete level2;
	delete model1;
	delete camera;

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

	switch (camType) // 1 = PAN (rotate around level) ||| 2 = STILL (bird's eye view) ||| 3 = CONTROL (free roam via user input)
	{
		default:
		case 1:
			gluLookAt(									// PanCam
				0, 6, 35,
				0, 6, 0,
				0, 1, 0);
			break;
		case 2:
			gluLookAt(							  	  // StillCam
				-2.5, 7, 30,						  // Zoom in / out (Eye position) 
				-2.5, 7, 0,							  // Origin (Point we are looking at)
				0, 1, 0);							  // (What vector points up)
			break;
		case 3:										  // ControlCam via WASD key inputs
			camera = new CameraController();
			break;
	}

	//-------TASK: TIME-------//		
	glutMotionFunc(Input::mousePositionUpdate); // Captures the keyboard import for GLUT camera
	glutPassiveMotionFunc(Input::mousePositionUpdate);
	glutKeyboardFunc(Input::keyboardUpdate);
	glutKeyboardUpFunc(Input::keyboardUpUpdate);
	//-----------------------//

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);   // RGB Alpha

	setupLighting();

	level1 = new Level("../Data/level.json", hasRoof, hasFloor, hasDoors); // SPECIFY STRING PATH TO LEVEL!!!
	level2 = new Level("../Data/level2.json", hasRoof, hasFloor, hasDoors); 
	model1 = new ModelWhole("../Models/statue", "statue");
	model1->setPosition(0, 0, 0);
	models.push_back(model1); // TASK 2: Add first model to array of models

	commandGiver(); // Give user hint of all possible commands
}

void controlDisplay() // CameraType == 3
{
	Time::update(); // TASK 2: Update time based on what we know
	Input::updateBefore(); // TASK 2: For camera

	commandManager();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Setup depth testing

	camera->update();

	glPushMatrix(); // For models
	{
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0.0f, -7.2f, 0.0f);
		drawModels(); // Draw / load all models in scene 
	}
	glPopMatrix();

	glPushMatrix(); // For levels
	{
		glScalef(5.0f, 2.5f, 5.0f);
		glTranslatef(0.0f, -1.5f, 0.0f);
		drawLevels(); // Load the currently selected level
	}
	glPopMatrix();

	Input::updateAfter(); // FOR CAMERA
	glutSwapBuffers();						// Swap frames and draw on the new screen frame
}

void stillDisplay() // CameraType == 2
{
	Time::update(); 
	Input::updateBefore(); 

	commandManager();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glPushMatrix(); // For models
	{
		glScalef(0.1f, 0.1f, 0.1f);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0); // 90 degrees for bird's eye view
		drawModels(); 
	}
	glPopMatrix();

	glPushMatrix(); // For levels
	{
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0); 
		drawLevels(); 
	}
	glPopMatrix();

	Input::updateAfter();
	glutSwapBuffers();	
}

void panDisplay() // CameraType == 1
{
	Time::update(); 
	Input::updateBefore(); 

	commandManager();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix(); // For models
	{
		glScalef(0.1f, 0.1f, 0.1f);
		glTranslatef(0, 0, 0);
		glRotatef(rotationOffset, 0, 1, 0);
		glRotatef(60, 1, 0, 0);
		drawModels();
	}
	glPopMatrix();

	glPushMatrix(); // For levels
	{
		glTranslatef(0, 0, 0);
		glRotatef(rotationOffset, 0, 1, 0);
		glRotatef(60, 1, 0, 0);
		drawLevels();
	}
	glPopMatrix();

	Input::updateAfter();
	rotationOffset += rotationSpeed;
	glutSwapBuffers();
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);		// Wait for 60th of a sec, then call timer again
}

void drawModels()
{
	// For each texture pointer in textures vector, Load the Models
	for (ModelWhole* model : models) { model->draw(); }
}

void drawLevels()
{
	if (activeLevel == 1) { level1->draw(); }
	if (activeLevel == 2) { level2->draw(); }
}

void commandCheck() // TASK 2 console commands -> will cause real-time change to the game engine scene
{
	string inputString, originalInputString;
	string roofState, doorState, floorState, dayState; // For control hub bools
	string modelName = "", coutName = "", levelName = "", properCommand = "";
	// (Name of model to spawn || name to print to console || name of level to load || proper command terminology)

	bool commandWasSuccessful = false; // If input worked & was viable
	bool levelChangingCommand = false; // If the command affects the level itself (e.g. walls or doors)

	if (hasRoof == true) { roofState = "hide roof"; }
	else { roofState = "show roof"; }

	if (hasFloor == true) { floorState = "hide floor"; }
	else { floorState = "show floor"; }

	if (hasDoors == true) { doorState = "hide doors"; }
	else { doorState = "show doors"; }

	if (hasLight == true && night == true) { dayState = "set daytime"; }
	else { dayState = "set nighttime"; }

	system("cls");

#pragma region GetUserInputAsString

	cout << "\n----------------------------------------\n----------------------------------------"
		<< "\n******Framewearer Command Console******"
		<< "\n\nPlease enter one of the following commands:"
		<< "\nspawn woman || spawn cat || spawn dog1 || spawn dog2"
		<< "\n             load level1 || load level2"
		<< "\n"
		<< "\nOr enter one of the following settings:"
		<< "\n        " << roofState << " || " << floorState << " || " << doorState
		<< "\n                    " << dayState
		<< "\n----------------------------------------"
		<< endl;

	getline(cin, inputString);
	originalInputString = inputString; // For showing the user their original input later (while inputString is processed to check for errors

	for_each(inputString.begin(), inputString.end(), [](char & c) // Convert string to lower case
	{
		c = ::tolower(c);
	});

	inputString.erase(remove(inputString.begin(), inputString.end(), ' '), inputString.end()); // Removes spaces from inside string

#pragma endregion

#pragma region ProcessUserInputString

	//------PROCESSING USER INPUT------//
	if ((inputString.find("woman") != string::npos)) // Woman
	{
		modelName = "statue"; // Actual model name for file retrieval
		coutName = "Woman"; // Codename for model when giving user console feedback
		properCommand = "spawn woman";
	}
	if ((inputString.find("dog1") != string::npos) || inputString == "spawndog" || inputString == "dog") // Dog
	{
		modelName = "dog1";
		coutName = "Dog 1";
		properCommand = "spawn dog1";
	}
	if ((inputString.find("dog2") != string::npos)) // Dog2
	{
		modelName = "dog2";
		coutName = "Dog 2";
		properCommand = "spawn dog2";
	}
	if ((inputString.find("cat") != string::npos)) // Cat
	{
		modelName = "cat1";
		coutName = "Cat";
		properCommand = "spawn cat";
	}
	if ((inputString.find("level1") != string::npos) || inputString == "level" || inputString == "spawnlevel") // Level1
	{
		levelName = "level1";
		coutName = "Level 1";
		properCommand = "spawn level1";
	}
	if ((inputString.find("level2") != string::npos)) // Level2
	{
		levelName = "level2";
		coutName = "Level 2";
		properCommand = "spawn level2";
	}

	//----------FOR BOOLS----------//
	if ((inputString.find("door") != string::npos)) // HasDoors
	{
		hasDoors = !hasDoors;
		coutName = "Doors";
		properCommand = doorState;
		commandWasSuccessful = true;
		levelChangingCommand = true;
	}
	if ((inputString.find("roof") != string::npos)) // HasRoof
	{
		hasRoof = !hasRoof;
		coutName = "Roofs";
		properCommand = roofState;
		commandWasSuccessful = true;
		levelChangingCommand = true;
	}
	if ((inputString.find("floor") != string::npos)) // HasFloor
	{
		hasFloor = !hasFloor;
		coutName = "Floors";
		properCommand = floorState;
		commandWasSuccessful = true;
		levelChangingCommand = true;
	}
	if ((inputString.find("day") != string::npos) || (inputString.find("night") != string::npos)) // HasLight
	{
		night = !night;
		hasLight = !hasLight;
		coutName = "Day / Night";
		properCommand = dayState;
		commandWasSuccessful = true;
		setupLighting(); // Update lighting in real time
	}

#pragma endregion

	if (inputString != "" && modelName != "") // Handling spawning of models
	{
		commandWasSuccessful = true;

		float xCoord, yCoord;
		cout << "\nPlease enter model X coordinate: ";
		cin >> xCoord;
		cout << "Please enter model Y coordinate: ";
		cin >> yCoord;

		cout << "\nSpawning Model [ " << coutName << " ] at [ X: " << xCoord << " ] [ Y: " << yCoord << " ]" << endl;

		ModelWhole* model = new ModelWhole("../Models/" + modelName, modelName);
		model->setPosition(xCoord, 0, yCoord);
		model->draw();

		models.push_back(model); // Add model to array of models
	}
	else if (inputString != "" && levelName != "") // Handling loading of levels
	{
		commandWasSuccessful = true;

		cout << "\nLoading Level [ " << coutName << " ]" << endl;

		if (coutName == "Level 1")
		{
			activeLevel = 1;
			level1 = new Level("../Data/level.json", hasRoof, hasFloor, hasDoors);

			level1->draw();
		}
		else if(coutName == "Level 2")
		{
			activeLevel = 2;
			level2 = new Level("../Data/level2.json", hasRoof, hasFloor, hasDoors);

			level2->draw();
		}
	}

	// OUTPUT END RESULT TO GIVE USER FEEDBACK ON COMMAND INPUT
	if (commandWasSuccessful == true && levelChangingCommand == false) // Good command -> Handles model spawning or level loading
	{
		cout << "\n----------------------------------------"
			<< "\nYour command: [ " << originalInputString << " ] >>>> [ " << properCommand << " ] was successful"
			<< "\n        Thank you for using the"
			<< "\n******Framewearer Command Console******"
			<< "\n----------------------------------------\n----------------------------------------" << endl;
	}
	else if (commandWasSuccessful == true && levelChangingCommand == true) // Good command -> Is level boolean related (hasDoors, hasRoof, hasFloor)
	{
		if (activeLevel == 1) { level1 = new Level("../Data/level.json", hasRoof, hasFloor, hasDoors); }
		else if (activeLevel == 2) { level2 = new Level("../Data/level2.json", hasRoof, hasFloor, hasDoors); }

		cout << "\n----------------------------------------"
			<< "\nYour command: [ " << originalInputString << " ] >>>> [ " << properCommand << " ] was successful"
			<< "\n        Thank you for using the"
			<< "\n******Framewearer Command Console******"
			<< "\n----------------------------------------\n----------------------------------------" << endl;
	}
	else if (inputString != "") // Bad command
	{
		cout << "\n----------------------------------------"
			<< "\nYour input: [ " << originalInputString << " ] is not a registered command"
			<< "\n----------------------------------------\n----------------------------------------" << endl;
	}
	else // Just pressed enter (probably to exit out of the console)
	{
		cout << "\n----------------------------------------"
			<< "\n******Exiting Framewearer Command Console******"
			<< "\n----------------------------------------\n----------------------------------------" << endl;
	}

	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	cout << "\nPress [ 5 ] for Help\n" << endl;
}

void commandGiver() // Simply reminds user of the optional commands and their button prompts
{
	cout << "\n-------------HELP CONSOLE---------------"
		<< "\n******Command List******"
		<< "\n\n[ 1 ] Camera Lock"
		<< "\n[ 2 ] Show FPS"
		<< "\n[ 3 ] Triangle Count"
		<< "\n[ 4 ] Input Command"
		<< "\n[ 5 ] Help?"
		<< "\n----------------------------------------"
		<< endl;
}

// For vertex and triangle count 
void calculateTriangleCount()
{
	float triangleCount = 0, vertexCount = 0, vertexCountModels = 0, vertexCountRooms = 0;

	cout << "\n-------------TRIANGLE COUNT---------------\n" << endl;

	for (ModelWhole* model : models) // Get vert count from every model
	{
		vertexCountModels += model->vertexCount;
		cout << "MODEL: " << model->vertexCount << endl;
	}

	if (activeLevel == 1) // Get vert count from the current level
	{
		vertexCountRooms += level1->vertexCount;
		cout << "LEVEL 1: " << level1->vertexCount << endl;
	}
	else if (activeLevel == 2)
	{
		vertexCountRooms += level2->vertexCount;
		cout << "LEVEL 2: " << level2->vertexCount << endl;
	}

	triangleCount = vertexCountModels / 3; // Every 3 model verts = 1 triangle face
	triangleCount += vertexCountRooms / 2; // Every 4 wall verts = 2 triangle faces
	vertexCount = vertexCountModels + vertexCountRooms;

	cout << "\n----------------------------------------\nTOTAL VERTS AMONGST MODELS: " << vertexCountModels << "\nTOTAL VERTS AMONGST ROOMS: " << vertexCountRooms
		 << "\n\n----------------------------------------\nTOTAL VERTEX COUNT: " << vertexCount << "\nTOTAL TRIANGLE COUNT: " << triangleCount 
		 << "\n----------------------------------------" << endl;
}

void commandManager()
{
	//----------FPS----------//												--- [ 2 ]
	if (Input::getKeyPressed('2')) // Flip bool value
	{
		spamFPS = !spamFPS;
	}
	if (spamFPS == true) // If true, spam console with FPS
	{
		std::cout << "FPS: " << Time::getFrameRate() << std::endl;
	}
	if (Input::getKeyReleased('2')) // At end of FPS spam, remind user of available commands
	{
		cout << "\nPress [ 5 ] for Help\n" << endl;
	}
	//-----------------------//

	//-------TRIANGLES-------//												--- [ 3 ]
	if (Input::getKeyPressed('3')) // On press, calc triangle total
	{
		system("cls");
		calculateTriangleCount();
	}
	if (Input::getKeyReleased('3')) // After showing tri count, remind user of available commands
	{
		cout << "\nPress [ 5 ] for Help\n" << endl;
	}
	//-----------------------//

	//-----INPUT CONSOLE-----//												--- [ 4 ]
	if (Input::getKeyPressed('4'))
	{
		system("cls");
		commandCheck();
	}
	//-----------------------//

	//----------HELP---------//												--- [ 5 ]
	if (Input::getKeyPressed('5'))
	{
		system("cls");
		commandGiver();
	}
	//-----------------------//
}

void setupLighting()
{
	lightSun.disableSun(); lightPoint.disablePoint(); lightSpot.disableSpot(); light1.disable(); light2.disable();

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
}

/*void calculateFPS() // WORKS, WAS COMMENTED OUT DUE TO CREATING A NEWER VERSION OF FPS CALCULATOR
{
	// For FPS
	frame_count++;
	final_time = time(NULL); // Returns seconds
	// If the time between initial & current time is > 1, a second has passed
	if (final_time - initial_time > 0)
	{
		// Calculate Frame Rate
		// Formula: frames drawn / time taken (in seconds) = fps
		std::cout << "AVERAGE FPS: " << frame_count / (final_time - initial_time) << std::endl;
		frame_count = 0; // Reset for next time we calculate (for every sec that passes)
		initial_time = final_time;
	}
}*/

// REFERENCES
// FPS counter
// https://www.youtube.com/watch?v=CYDFQ1oJJdI
//
// GLUT warp pointer (for moving mouse position to middle of screen on game engine startup)
// https://gamedev.net/forums/topic/612651-help-understanding-how-to-get-glutwarppointer-to-work-right/4873154/
// 
// To Lower() for string input
// https://www.codegrepper.com/code-examples/cpp/how+to+convert+whole+string+into+lowercase+in+c%2B%2B
//
// Erase spaces from string
// https://www.codegrepper.com/code-examples/cpp/c%2B%2B+remove+spaces+from+string
//
// String .contains c++ equivalent (for input commands)
// https://www.codegrepper.com/code-examples/cpp/how+to+check+if+a+string+contains+a+certain+word+in+c%2B%2B