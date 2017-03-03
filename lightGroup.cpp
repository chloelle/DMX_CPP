//class implementation for group of DMX lights (making up your DMX universe)

#include "stdafx.h"
#include "lightGroup.h"
#include "DMXLight.h"
#include <iostream>
#include <time.h> 

lightGroup::lightGroup(){
	std::vector<DMXLight*> lights_; 
	lights = lights_;
}

void lightGroup::addLight(DMXLight &light){
	lights.push_back(&light); 
}

bool lightGroup::removeLight(DMXLight &light){
	bool notFound = true;
	for (unsigned int p = 0; p < lights.size(); p++){
		if (&light == lights[p]){
			lights.erase(lights.begin() + p); 
			return false; 
		}
	}
	return notFound; 
}

int parseRGBAWStringInput(std::string input){
	if ( input == "r" || input == "R" || input == "red" || input == "Red"){
		return 0; //red
	}
	else if (input == "g" || input == "G" || input == "green" || input == "Green"){
		return 1; 
	}
	else if(input == "b" || input == "B" || input == "blue" || input == "Blue"){
		return 2;
	}
	else if(input == "a" || input == "A" || input == "amber" || input == "Amber"){
		return 3;
	}
	else if(input == "w" || input == "W" || input == "white" || input == "White"){
		return 4;
	}
	else{
		std::cout << "invalid input color name." << std::endl;
		return -1;
	}
}

void lightGroup::allOnSingleColor(OpenDMX &myOpenDMX, std::string colorChannel, int numLoops, DWORD milliseconds){

	int channel = parseRGBAWStringInput(colorChannel);

	if (channel >= 0){
		for (int i = 0; i < numLoops; i++){
			int p = 0;
			while (p <= 50){

				myOpenDMX.initOpenDMX();
				myOpenDMX.zerosDMXValue();

				for (unsigned int q = 0; q < lights.size(); q++){ //loop through lights
					if (lights[q]->getHasDimmingChannel()){
						myOpenDMX.setDMXValue(lights[q]->getDimmingChannel(), 255); 
					}
					switch(channel){
					case 0: myOpenDMX.setDMXValue(lights[q]->getRChannel(), 255); break;
					case 1: myOpenDMX.setDMXValue(lights[q]->getGChannel(), 255); break;
					case 2: myOpenDMX.setDMXValue(lights[q]->getBChannel(), 255); break;

					case 3: if (lights[q]->getHasAmberChannel()){
						myOpenDMX.setDMXValue(lights[q]->getAChannel(), 255); }
							break;

					case 4: if (lights[q]->getHasWhiteChannel()){
						myOpenDMX.setDMXValue(lights[q]->getWChannel(), 255); }
							break;

					default: ;
					}
				}

				myOpenDMX.writeData();
				Sleep(milliseconds);
				p++;

			}
		}
	}
}

void lightGroup::crossDissolve(OpenDMX &myOpenDMX, int numSteps, DWORD milliseconds, RGBxx &firstColor, RGBxx &lastColor){

	RGBxx difference = lastColor - firstColor; 
	RGBxx stepVector = scalarMultiply(difference, float(1.0)/float(numSteps));

	for (int i = 0; i < numSteps; i++){
		int p = 0;
		RGBxx updateColor = firstColor + scalarMultiply(stepVector, float(i)); 
		while (p <= 2){
			myOpenDMX.initOpenDMX();
			myOpenDMX.zerosDMXValue();
			for (unsigned int q = 0; q < lights.size(); q++){
				if (lights[q]->getHasDimmingChannel()){
					myOpenDMX.setDMXValue(lights[q]->getDimmingChannel(), 255);
				}
				myOpenDMX.setDMXValue(lights[q]->getRChannel(), updateColor.R); 
				myOpenDMX.setDMXValue(lights[q]->getGChannel(), updateColor.G); 
				myOpenDMX.setDMXValue(lights[q]->getBChannel(), updateColor.B); 
				if (lights[q]->getHasWhiteChannel()){
					myOpenDMX.setDMXValue(lights[q]->getWChannel(), updateColor.W); 
				}
				if (lights[q]->getHasAmberChannel()){
					myOpenDMX.setDMXValue(lights[q]->getAChannel(), updateColor.A); 
				}
			}
			//myOpenDMX.setDMXValue(A_channel, 0); 
			myOpenDMX.writeData();
			Sleep(milliseconds);
			p++;
		}
	}
}

void lightGroup::rainbowCrossDissolve(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	//RGBxx(int numChannels_, int r, int g, int b, int a, int w);
	RGBxx red = RGBxx::RGBxx(4, 255, 0, 0, 0, 0);
	RGBxx yellow = RGBxx::RGBxx(4, 128, 128, 0, 0, 0);
	RGBxx green = RGBxx::RGBxx(4, 0, 255, 0, 0, 0);
	RGBxx cyan = RGBxx::RGBxx(4, 0, 128, 128, 0, 0);
	RGBxx blue = RGBxx::RGBxx(4, 0, 0, 255, 0, 0);
	RGBxx violet =  RGBxx::RGBxx(4, 128, 0, 128, 0, 0);

	for (int i = 0; i < numLoops; i++){
		crossDissolve(myOpenDMX, 20, milliseconds, red, yellow); 
		crossDissolve(myOpenDMX, 20, milliseconds, yellow, green); 
		crossDissolve(myOpenDMX, 20, milliseconds, green, cyan); 
		crossDissolve(myOpenDMX, 20, milliseconds, cyan, blue); 
		crossDissolve(myOpenDMX, 20, milliseconds, blue, red); 
	}
}

void lightGroup::allColorsRampDown(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	for (int i = 0; i < numLoops; i++){
		int p = 0;

		while (p <= 255){
			//this is for an example with an 8-channel DMX light
			myOpenDMX.initOpenDMX();
			myOpenDMX.zerosDMXValue();

			for (unsigned int q = 0; q < lights.size(); q++){
				if (lights[q]->getHasDimmingChannel()){
					myOpenDMX.setDMXValue(lights[q]->getDimmingChannel(), 255); //4 /255
				}

				myOpenDMX.setDMXValue(lights[q]->getRChannel(), 255-p); //5 R
				myOpenDMX.setDMXValue(lights[q]->getGChannel(), 255-p); //6 G
				myOpenDMX.setDMXValue(lights[q]->getBChannel(), 255-p); //7 B

				if (lights[q]->getHasWhiteChannel()){
					myOpenDMX.setDMXValue(lights[q]->getWChannel(), 255-p); //8 W
				}
				if (lights[q]->getHasAmberChannel()){
					myOpenDMX.setDMXValue(lights[q]->getAChannel(), 255-p); //8 W
				}
			}
			myOpenDMX.writeData();
			Sleep(milliseconds);
			p++;
		}
	}
}

void lightGroup::randomPattern(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	srand (time(NULL));

	for (int i = 0; i < numLoops; i++){
		int p = 0;

		while (p <= 255){
			//this is for an example with an 8-channel DMX light
			myOpenDMX.initOpenDMX();
			myOpenDMX.zerosDMXValue();

			for (unsigned int q = 0; q < lights.size(); q++){
				if (lights[q]->getHasDimmingChannel()){
					myOpenDMX.setDMXValue(lights[q]->getDimmingChannel(), 255); //4 /255
				}

				myOpenDMX.setDMXValue(lights[q]->getRChannel(), rand() % 256); //5 R
				myOpenDMX.setDMXValue(lights[q]->getGChannel(), rand() % 256); //6 G
				myOpenDMX.setDMXValue(lights[q]->getBChannel(), rand() % 256); //7 B

				if (lights[q]->getHasWhiteChannel()){
					myOpenDMX.setDMXValue(lights[q]->getWChannel(), rand() % 256); //8 W
				}
				if (lights[q]->getHasAmberChannel()){
					myOpenDMX.setDMXValue(lights[q]->getAChannel(), rand() % 256); //8 W
				}
			}
			myOpenDMX.writeData();
			Sleep(milliseconds);
			p++;
		}
	}
}