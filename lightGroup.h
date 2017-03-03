//class definition for group of DMX lights (making up your DMX universe)

#ifndef LIGHTGROUP_H
#define LIGHTGROUP_H

#include <vector>
#include "DMXLight.h"
#include <windows.h>

class lightGroup{
public:
	std::vector<DMXLight*> lights; 

public:
	lightGroup(); 
	void addLight(DMXLight &light);
	bool removeLight(DMXLight &light); 
	void allOnSingleColor(OpenDMX &myOpenDMX, std::string colorChannel, int numLoops, DWORD milliseconds);
	void crossDissolve(OpenDMX &myOpenDMX, int numSteps, DWORD milliseconds, RGBxx &firstColor, RGBxx &lastColor);
	void rainbowCrossDissolve(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void allColorsRampDown(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void randomPattern(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
};

#endif