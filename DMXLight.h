//class for a DMX light with RGB, RGBW, or RGBAW channels

#ifndef DMXLIGHT_H
#define DMXLIGHT_H

#include "OpenDMX.h"
#include "RGBxx.h"
#include <windows.h>
#include <string>

//Class for a multi-channel DMX light
class DMXLight{
private:
	int startchannel; //programmed on the device with buttons
	int numChannels; 
	int numColors;
	std::string lightNickname; 
	
	int R_channel;
	int G_channel;
	int B_channel; 
	int A_channel;
	int W_channel; 
	int dimming_channel;  //some lights have a master dimming channel that needs to be set to 255 to use the others.

	bool hasDimmingChannel;
	bool hasAmberChannel; 
	bool hasWhiteChannel;

	bool initializedDimming; 
	bool initializedAmber;
	bool initializedWhite;
	
public:
	DMXLight(std::string lightNickname, int startchannel_);
	DMXLight(std::string lightNickname, int startchannel_, int numColors_); 
	DMXLight(std::string lightNickname, int startchannel_, int numColors_, int numChannels_);

	void setNumChannels(int numChannels_);
	void setNumColors(int numColors_);
	void setRChannel(int R_channel_);
	void setGChannel(int G_channel_); 
	void setBChannel(int B_channel_);
	void setAChannel(int A_channel_); 
	void setWChannel(int W_channel_);
	void setHasDimmingChannel(bool hasDimming_); 
	void setHasWhiteChannel(bool hasWhite_); 
	void setHasAmberChannel(bool hasAmber_); 
	void setDimmingChannel(int dimming_channel_); 

	int getNumChannels();
	int getNumColors();
	int getRChannel();
	int getGChannel(); 
	int getBChannel();
	int getAChannel(); 
	int getWChannel();
	bool getHasDimmingChannel(); 
	int getDimmingChannel(); 
	bool getHasWhiteChannel();
	bool getHasAmberChannel();
	bool setAllBooleans();

	void allColorsRampDown(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void allRedOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void allGreenOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void allBlueOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void allAmberOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void allWhiteOn(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	void allOnSingleChannel(OpenDMX &myOpenDMX, int channel, int numLoops, DWORD milliseconds);
	void randomPattern(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds); 
	void crossDissolve(OpenDMX &myOpenDMX, int numSteps, DWORD milliseconds, RGBxx &firstColor, RGBxx &lastColor);
	void rainbowCrossDissolve(OpenDMX &myOpenDMX, int numLoops, DWORD milliseconds);
	bool lightCorrectlyInitialized(); 

};

#endif