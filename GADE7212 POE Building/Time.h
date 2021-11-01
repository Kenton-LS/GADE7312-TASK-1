#pragma once
// It's about time....
// Using this class is time consuming....

static class Time // Static because we reference the classes directly through here, not an instance of the time object
{
private:
	static int prevTime; // Time engine has been running for since program's beginning and ending on previous frame (in MILLISECONDS)
	static int time; // Current time as of current frame being rendered (can be used to get difference between prev and current frame -- thus giving deltaTime)
public:
	static void update(); // Update time and prev time variables
	static float getTime(); // Calculate time as a floating point value in SECONDS
	static float getDeltaTime(); // Calc time in MILLISECONDS
	static float getFrameRate();
};