//implementation for class DMXLight

#include "stdafx.h"
#include "DMXLight.h"
#include "RGBxx.h"

#include <iostream>
#include <time.h> 

bool DMXLight::lightCorrectlyInitialized(){
	bool correctlyInitalized = true; 

	if (numChannels < 0){
		std::cout << "Please initialize numChannels for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}
	if (numColors  < 0){
		std::cout << "Please initialize numColors for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}
	if (!initializedDimming){
		std::cout << "Please initialize hasDimmingChannel for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}
	if (!initializedAmber){
		std::cout << "Please set amber channel for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}
	if (!initializedWhite){
		std::cout << "Please set white channel for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}
	if (R_channel < 0 ){
		std::cout << "Please set red channel for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}
	if (G_channel < 0 ){
		std::cout << "Please set green channel for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}
	if (B_channel < 0 ){
		std::cout << "Please set blue channel for light " << lightNickname << std::endl;
		correctlyInitalized = false; 
	}

	return correctlyInitalized;
}

DMXLight::DMXLight(std::string lightNickname_, int startchannel_){
	lightNickname = lightNickname_;
	startchannel = startchannel_;
	numColors = -1;
	numChannels = -1;

	R_channel = -1;
	G_channel = -1;
	B_channel = -1;
	A_channel = -1;
	W_channel = -1;
	dimming_channel = -1; 
	hasDimmingChannel = 0; 
	hasAmberChannel = 0;
	hasWhiteChannel = 0;

	initializedDimming = 0;
	initializedAmber = 0; 
	initializedWhite = 0; 
}

DMXLight::DMXLight(std::string lightNickname_, int startchannel_, int numColors_){
	lightNickname = lightNickname_;
	startchannel = startchannel_;
	numColors = numColors_;
	numChannels = -1;

	R_channel = -1;
	G_channel = -1;
	B_channel = -1;
	A_channel = -1;
	W_channel = -1;
	dimming_channel = -1;

	hasDimmingChannel = 0; 
	hasAmberChannel = 0;
	hasWhiteChannel = 0;

	initializedDimming = 0;
	initializedAmber = 0; 
	initializedWhite = 0; 

}

DMXLight::DMXLight(std::string lightNickname_, int startchannel_, int numColors_, int numChannels_){
	lightNickname = lightNickname_;
	startchannel = startchannel_;
	numColors = numColors_;
	numChannels = numChannels_;

	R_channel = -1;
	G_channel = -1;
	B_channel = -1;
	A_channel = -1;
	W_channel = -1;
	dimming_channel = -1; 
	
	hasDimmingChannel = 0; 
	hasAmberChannel = 0;
	hasWhiteChannel = 0;

	initializedDimming = 0;
	initializedAmber = 0; 
	initializedWhite = 0; 

}

void DMXLight::setNumChannels(int numChannels_){
	numChannels = numChannels_;
}

void DMXLight::setNumColors(int numColors_){
	numColors = numColors_;
}

void DMXLight::setRChannel(int R_channel_){
	R_channel = startchannel + R_channel_ - 1;
}

void DMXLight::setGChannel(int G_channel_){
	G_channel = startchannel + G_channel_ - 1;
}

void DMXLight::setBChannel(int B_channel_){
	B_channel = startchannel + B_channel_ - 1;
}

void DMXLight::setAChannel(int A_channel_){
	A_channel = startchannel + A_channel_ - 1;
	hasAmberChannel = true; 
	initializedAmber = true; 
}

void DMXLight::setWChannel(int W_channel_){
	W_channel = startchannel + W_channel_ - 1;
	hasWhiteChannel = true;
	initializedWhite = true; 
}

void DMXLight::setDimmingChannel(int dimming_channel_){
	dimming_channel = startchannel + dimming_channel_ - 1;
	hasDimmingChannel = true;
	initializedDimming = true;
}

void DMXLight::setHasDimmingChannel(bool hasDimming_){
	hasDimmingChannel = hasDimming_;
	initializedDimming = true;
}

void DMXLight::setHasWhiteChannel(bool hasWhite_){
	hasWhiteChannel = hasWhite_;
	initializedWhite = true; 
}

void DMXLight::setHasAmberChannel(bool hasAmber_){
	hasAmberChannel = hasAmber_;
	initializedAmber = true; 
}


int DMXLight::getNumChannels(){
	return numChannels;
}

int DMXLight::getNumColors(){
	return numColors;
}

int DMXLight::getRChannel(){
	return R_channel;
}

int DMXLight::getGChannel(){
	return G_channel;
}

int DMXLight::getBChannel(){
	return B_channel;
}

int DMXLight::getAChannel(){
	return A_channel;
}

int DMXLight::getWChannel(){
	return W_channel;
}

bool DMXLight::getHasDimmingChannel(){
	return hasDimmingChannel;
}

bool DMXLight::getHasWhiteChannel(){
	return hasWhiteChannel;
}

bool DMXLight::getHasAmberChannel(){
	return hasAmberChannel;
}

int DMXLight::getDimmingChannel(){
	return dimming_channel;
}

void DMXLight::crossDissolve(OpenDMX &myOpenDMX, int numSteps, DWORD milliseconds, RGBxx &firstColor, RGBxx &lastColor){

	if (lightCorrectlyInitialized()){
		RGBxx difference = lastColor - firstColor; 
		RGBxx stepVector = scalarMultiply(difference, float(1.0)/float(numSteps));

		for (int i = 0; i < numSteps; i++){
			int p = 0;
			RGBxx updateColor = firstColor + scalarMultiply(stepVector, float(i)); 
			while (p <= 2){
				myOpenDMX.initOpenDMX();
				myOpenDMX.zerosDMXValue();

				if (hasDimmingChannel){
					myOpenDMX.setDMXValue(dimming_channel, 255);
				}

				myOpenDMX.setDMXValue(R_channel, updateColor.R); 
				myOpenDMX.setDMXValue(G_channel, updateColor.G); 
				myOpenDMX.setDMXValue(B_channel, updateColor.B); 

				if (hasWhiteChannel){
					myOpenDMX.setDMXValue(W_channel, updateColor.W); 
				}
				if (hasAmberChannel){
					myOpenDMX.setDMXValue(A_channel, updateColor.A); 
				}

				myOpenDMX.writeData();
				Sleep(milliseconds);
				p++;
			}
		}
	}
}

void DMXLight::rainbowCrossDissolve(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
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

void DMXLight::randomPattern(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){

	if (lightCorrectlyInitialized()){
		/* initialize random seed: */ 
		//modulo operation does not generate uniformly distributed random numbers in the span but it's fine for testing

		srand (time(NULL));

		for (int i = 0; i < numLoops; i++){
			int p = 0;

			while (p <= 50){

				myOpenDMX.initOpenDMX();
				myOpenDMX.zerosDMXValue();

				if (hasDimmingChannel){
					myOpenDMX.setDMXValue(dimming_channel, 255); //4 /255
				}

				myOpenDMX.setDMXValue(R_channel, rand() % 256); //5 R
				myOpenDMX.setDMXValue(G_channel, rand() % 256); //6 G
				myOpenDMX.setDMXValue(B_channel, rand() % 256); //7 B

				if (hasWhiteChannel){
					myOpenDMX.setDMXValue(W_channel, rand() % 256); //8 W
				}
				if (hasAmberChannel){
					myOpenDMX.setDMXValue(A_channel, rand() % 256); //8 W
				}
				myOpenDMX.writeData();
				Sleep(milliseconds);
				p++;
			}
		}
	}
}

void DMXLight::allColorsRampDown(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	if (lightCorrectlyInitialized()){
		for (int i = 0; i < numLoops; i++){
			int p = 0;

			while (p <= 255){
				//this is for an example with an 8-channel DMX light
				myOpenDMX.initOpenDMX();
				myOpenDMX.zerosDMXValue();

				if (hasDimmingChannel){
					myOpenDMX.setDMXValue(dimming_channel, 255); //4 /255
				}

				myOpenDMX.setDMXValue(R_channel, 255-p); //5 R
				myOpenDMX.setDMXValue(G_channel, 255-p); //6 G
				myOpenDMX.setDMXValue(B_channel, 255-p); //7 B

				if (hasWhiteChannel){
					myOpenDMX.setDMXValue(W_channel, 255-p); //8 W
				}
				if (hasAmberChannel){
					myOpenDMX.setDMXValue(A_channel, 255-p); //8 W
				}

				myOpenDMX.writeData();
				Sleep(milliseconds);
				p++;
			}
		}
	}
}

void DMXLight::allOnSingleChannel(OpenDMX &myOpenDMX, int channel, int numLoops, DWORD milliseconds){
	if (lightCorrectlyInitialized()){
		for (int i = 0; i < numLoops; i++){
			int p = 0;
			while (p <= 50){
				myOpenDMX.initOpenDMX();
				myOpenDMX.zerosDMXValue();

				if (hasDimmingChannel){
					myOpenDMX.setDMXValue(dimming_channel, 255);
				}

				myOpenDMX.setDMXValue(channel, 255); 
				myOpenDMX.writeData();
				Sleep(milliseconds);
				p++;
			}
		}
	}
}

void DMXLight::allRedOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	allOnSingleChannel(myOpenDMX, R_channel, numLoops, milliseconds);
}

void DMXLight::allGreenOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	allOnSingleChannel(myOpenDMX, G_channel, numLoops, milliseconds);
}

void DMXLight::allBlueOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	allOnSingleChannel(myOpenDMX, B_channel, numLoops, milliseconds);
}

void DMXLight::allWhiteOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	if (hasWhiteChannel){
		allOnSingleChannel(myOpenDMX, W_channel, numLoops, milliseconds);
	}
	else{
		std::cout << "No white channel initalized. Aborting allWhiteOn command for light: "<< lightNickname << std::endl;
	}
}

void DMXLight::allAmberOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds){
	if (hasAmberChannel){
		allOnSingleChannel(myOpenDMX, A_channel, numLoops, milliseconds);
	}
	else{
		std::cout << "No amber channel initalized. Aborting allAmberOn command for light: "<< lightNickname << std::endl;
	}
}
