//Adapted from C# code from Linked2 Software (www.Linked2Software.com) Building Lighting Control Systems
//DMX-512 Test Application Visual Studio 2012 C++
//Original C# written by Richard A. Blackwell with example source provided by Enttec.com
//New C++ adapation written by Chloe A. LeGendre
//
//ref: DMX controller with FTDI driver: https://www.enttec.com/?main_menu=Products&pn=70303
//ref: 64-bit .dll installed in C:/Windows/System32 from http://www.ftdichip.com/Drivers/CDM/CDM21218_Setup.zip
//ref: DLLs the Dynamic Way https://msdn.microsoft.com/en-us/library/ms810279.aspx
//ref: what is DMX? http://dmxusb.com/what-is-dmx/

//ref: light 1 example RGBW: https://www.amazon.com/Coidak-CO820-12LEDs-Colorful-DMX512/dp/B01CTVHUAK/
//ref: light 2 example RGBAW: http://www.blizzardlighting.com/products/par-cans-and-wash-lights/item/355-hotbox-5-rgbaw

#include "stdafx.h"
#include <iostream>

#include "RGBxx.h"
#include "OpenDMX.h"
#include "DMXLight.h"
#include "lightGroup.h"

//required for DLL loading and some data types
#include <windows.h>

//load DLL   **CHANGE FILENAME HERE**
bool loadDLL(HINSTANCE &dllHandle){
	//absolute path filename of DLL **CHANGE HERE**
	wchar_t* file = L"C:\\Users\\legendre\\Documents\\Visual Studio 2012\\Projects\\DMX_CPP\\DMX_CPP\\ftd2xx.dll";
	//try to load DLL
	dllHandle = LoadLibrary(file);

	if ( NULL == dllHandle) {
		std::cout << "Could not load the dynamic library" << std::endl;
		return true;
	}
	else return false; //success
}

int main(int argc, char* argv[]){

	DWORD milliseconds = 25; //suggested interval for DMX

	bool isVerbose = false; //set to true for logging DMX status to console for debugging purposes
	OpenDMX myOpenDMX = OpenDMX::OpenDMX(isVerbose);

	HINSTANCE dllHandle_ = NULL;   
	bool failure = loadDLL(dllHandle_);

	if (!failure){
		myOpenDMX.dllHandle = dllHandle_;
		failure = myOpenDMX.start();

		if (!failure){ //if we successfully started DMX communication.

			bool failure = myOpenDMX.initOpenDMX();

			if (failure){
				return failure;
			}

			//---------
			//make a light. RGBW example (Coidak)
			//---------
			int light1_startIDX = 1; //program this number with buttons on the light - the DMX light address 

			//this information comes from the help booklet for your light, describing the DMX channels used by the light. 
			int light1_numChannels = 8; 
			int light1_numColors = 4;  //RGBW
			bool light1_has_master_dimming = true; 
			bool light1_has_amber_channel = false; 
			int light1_master_dimming_channel = 4;
			int light1_red_dimming_channel = 5; 
			int light1_green_dimming_channel = 6; 
			int light1_blue_dimming_channel = 7; 
			int light1_white_dimming_channel = 8; 

			//constructor and initialization
			DMXLight myRGBWLight = DMXLight::DMXLight("RGBW Coidak", light1_startIDX, light1_numColors, light1_numChannels);
			myRGBWLight.setRChannel(light1_red_dimming_channel);
			myRGBWLight.setGChannel(light1_green_dimming_channel);
			myRGBWLight.setBChannel(light1_blue_dimming_channel);
			myRGBWLight.setWChannel(light1_white_dimming_channel);
			myRGBWLight.setHasDimmingChannel(light1_has_master_dimming);
			myRGBWLight.setDimmingChannel(light1_master_dimming_channel);
			myRGBWLight.setHasAmberChannel(light1_has_amber_channel);
			if (!myRGBWLight.lightCorrectlyInitialized()){
				system("pause"); 
				return 1; 
			}

			//---------
			//make a light. RGBAW example (Blizzard hotbox)
			//---------
			int light2_startIDX = 9; //program this number with buttons on the light - the DMX light address 

			//this information comes from the help booklet for your light, describing the DMX channels used by the light. 
			int light2_numChannels = 5;
			int light2_numColors = 5; 
			bool light2_has_master_dimming = false;
			int light2_red_dimming_channel = 1; 
			int light2_green_dimming_channel = 2; 
			int light2_blue_dimming_channel = 3; 
			int light2_amber_dimming_channel = 4; 
			int light2_white_dimming_channel = 5; 

			//constructor and initialization
			DMXLight myRGBAWLight = DMXLight::DMXLight("RGBAW Blizzard Hotbox", light2_startIDX, light2_numColors, light2_numChannels);
			myRGBAWLight.setRChannel(light2_red_dimming_channel);
			myRGBAWLight.setGChannel(light2_green_dimming_channel);
			myRGBAWLight.setBChannel(light2_blue_dimming_channel);
			myRGBAWLight.setAChannel(light2_amber_dimming_channel);
			myRGBAWLight.setWChannel(light2_white_dimming_channel);
			myRGBAWLight.setHasDimmingChannel(light2_has_master_dimming);
			if (!myRGBAWLight.lightCorrectlyInitialized()){
				system("pause"); 
				return 1; 
			}

			/*
			//---------
			//test some automatic functions for light 1 only
			//---------
			myRGBWLight.allColorsRampDown(myOpenDMX,1,milliseconds);
			myRGBWLight.allRedOn(myOpenDMX,1,milliseconds);
			myRGBWLight.allBlueOn(myOpenDMX,1,milliseconds);
			myRGBWLight.allGreenOn(myOpenDMX,1,milliseconds);
			myRGBWLight.allWhiteOn(myOpenDMX,1,milliseconds);
			myRGBWLight.randomPattern(myOpenDMX,1,milliseconds);
			myRGBWLight.rainbowCrossDissolve(myOpenDMX,2,milliseconds);
			*/

			/*
			//---------
			//test some automatic functions for light 2 only
			//---------
			myRGBAWLight.allColorsRampDown(myOpenDMX,1,milliseconds);
			myRGBAWLight.allRedOn(myOpenDMX,1,milliseconds);
			myRGBAWLight.allBlueOn(myOpenDMX,1,milliseconds);
			myRGBAWLight.allGreenOn(myOpenDMX,1,milliseconds);
			myRGBAWLight.allWhiteOn(myOpenDMX,1,milliseconds);
			myRGBAWLight.randomPattern(myOpenDMX,1,milliseconds);
			myRGBAWLight.rainbowCrossDissolve(myOpenDMX,2,milliseconds);
			*/

			//---------
			//make a light group and add lights to it
			//---------
			lightGroup myLights = lightGroup::lightGroup(); 
			myLights.addLight(myRGBWLight);
			myLights.addLight(myRGBAWLight); 

			/*
			//---------
			//test some automatic functions for light group
			//---------
			myLights.allOnSingleColor(myOpenDMX, "r", 1, milliseconds);
			myLights.allOnSingleColor(myOpenDMX, "g", 1, milliseconds);
			myLights.allOnSingleColor(myOpenDMX, "b", 1, milliseconds);
			myLights.allOnSingleColor(myOpenDMX, "a", 1, milliseconds);
			myLights.allOnSingleColor(myOpenDMX, "w", 1, milliseconds);
			myLights.rainbowCrossDissolve(myOpenDMX,1,milliseconds); 
			*/

			/*
			//---------
			//test remove light//
			//---------
			myLights.removeLight(myRGBWLight); 
			myLights.allOnSingleColor(myOpenDMX, "g", 1, milliseconds);
			myLights.allOnSingleColor(myOpenDMX, "b", 1, milliseconds);
			*/

			//command line IO
			int lightIDXinput; 
			bool outerDoContinue = true; 
			while (outerDoContinue){
				std::cout << "Enter a light. Options are: " << std::endl;
				std::cout << "all lights (0), "; 
				std::cout << "light1 only (1), "; 
				std::cout << "light2 only (2) "; 
				std::cout << "QUIT (-1)" << std::endl;
				std::cin >> lightIDXinput; 

				switch (lightIDXinput){
				case 0:{
					int input; 
					bool doContinue = true; 
					while(doContinue){
						std::cout << "Enter a lighting command. Options are:" << std::endl;
						std::cout << "all colors ramp down (1), ";
						std::cout << "all red on (2), " ;
						std::cout << "all green on (3), " ;
						std::cout << "all blue on (4), " ;
						std::cout << "all amber on (5), " ;
						std::cout << "all white on (6), " ;
						std::cout << "random pattern (7), " ;
						std::cout << "rainbow cross dissolve (8), ";
						std::cout << "quit (0)." << std::endl; 
						std::cin >> input;

						switch (input){
						case 1: myLights.allColorsRampDown(myOpenDMX, 1, milliseconds); break;
						case 2: myLights.allOnSingleColor(myOpenDMX, "r", 1, milliseconds); break; 
						case 3: myLights.allOnSingleColor(myOpenDMX, "g", 1, milliseconds); break;
						case 4: myLights.allOnSingleColor(myOpenDMX, "b", 1, milliseconds); break; 
						case 5:	myLights.allOnSingleColor(myOpenDMX, "a", 1, milliseconds); break; 
						case 6: myLights.allOnSingleColor(myOpenDMX, "w", 1, milliseconds); break; 
						case 7: myLights.randomPattern(myOpenDMX,1,milliseconds); break; 
						case 8: myLights.rainbowCrossDissolve(myOpenDMX,1,milliseconds); break;
						case 0: doContinue = false; break; 
						default: std::cout << "Invalid argument. please select again." << std::endl; 
						}
					} //end while doCont	
					break;
					   }
				case 1:{
					int input; 
					bool doContinue = true; 
					while(doContinue){
						std::cout << "Enter a lighting command. Options are:" << std::endl;
						std::cout << "all colors ramp down (1), ";
						std::cout << "all red on (2), " ;
						std::cout << "all green on (3), " ;
						std::cout << "all blue on (4), " ;
						std::cout << "all amber on (5), " ;
						std::cout << "all white on (6), " ;
						std::cout << "random pattern (7), " ;
						std::cout << "rainbow cross dissolve (8), ";
						std::cout << "quit (0)." << std::endl; 
						std::cin >> input;

						switch (input){
						case 1: myRGBWLight.allColorsRampDown(myOpenDMX,1,milliseconds); break;
						case 2: myRGBWLight.allRedOn(myOpenDMX,1,milliseconds); break; 
						case 3: myRGBWLight.allGreenOn(myOpenDMX,1,milliseconds); break;
						case 4: myRGBWLight.allBlueOn(myOpenDMX,1,milliseconds); break; 
						case 5: myRGBWLight.allAmberOn(myOpenDMX,1,milliseconds); break; 
						case 6: myRGBWLight.allWhiteOn(myOpenDMX,1,milliseconds); break; 
						case 7: myRGBWLight.randomPattern(myOpenDMX,1,milliseconds); break; 
						case 8: myRGBWLight.rainbowCrossDissolve(myOpenDMX,1,milliseconds); break;
						case 0: doContinue = false; break; 
						default: std::cout << "Invalid argument. please select again." << std::endl; 
						}
					} //end while doCont	
					break;
					   }
				case 2: {
					int input; 
					bool doContinue = true; 
					while(doContinue){
						std::cout << "Enter a lighting command. Options are:" << std::endl;
						std::cout << "all colors ramp down (1), ";
						std::cout << "all red on (2), " ;
						std::cout << "all green on (3), " ;
						std::cout << "all blue on (4), " ;
						std::cout << "all amber on (5), " ;
						std::cout << "all white on (6), " ;
						std::cout << "random pattern (7), " ;
						std::cout << "rainbow cross dissolve (8), ";
						std::cout << "quit (0)." << std::endl; 
						std::cin >> input;

						switch (input){
						case 1: myRGBAWLight.allColorsRampDown(myOpenDMX,1,milliseconds); break;
						case 2: myRGBAWLight.allRedOn(myOpenDMX,1,milliseconds); break; 
						case 3: myRGBAWLight.allGreenOn(myOpenDMX,1,milliseconds); break;
						case 4: myRGBAWLight.allBlueOn(myOpenDMX,1,milliseconds); break; 
						case 5: myRGBAWLight.allAmberOn(myOpenDMX,1,milliseconds); break; 
						case 6: myRGBAWLight.allWhiteOn(myOpenDMX,1,milliseconds); break; 
						case 7: myRGBAWLight.randomPattern(myOpenDMX,1,milliseconds); break; 
						case 8: myRGBAWLight.rainbowCrossDissolve(myOpenDMX,1,milliseconds); break;
						case 0: doContinue = false; break; 
						default: std::cout << "Invalid argument. please select again." << std::endl; 
						}
					} //end while doCont	
					break;
						}
				case -1: outerDoContinue = false; break; 
				default: std::cout << "Invalid argument. please select again." << std::endl; 

				}
			}
			/*
			int input; 
			bool doContinue = true; 
			while(doContinue){
				std::cout << "Enter a command. Options are:" << std::endl;
				std::cout << "all colors ramp down (1), ";
				std::cout << "all red on (2), " ;
				std::cout << "all green on (3), " ;
				std::cout << "all blue on (4), " ;
				std::cout << "all amber on (5), " ;
				std::cout << "all white on (6), " ;
				std::cout << "random pattern (7), " ;
				std::cout << "rainbow cross dissolve (8), ";
				std::cout << "quit (0)." << std::endl; 
				std::cin >> input;

				switch (input){
				case 1: myRGBWLight.allColorsRampDown(myOpenDMX,1,milliseconds); break;
				case 2: myRGBWLight.allRedOn(myOpenDMX,1,milliseconds); break; 
				case 3: myRGBWLight.allGreenOn(myOpenDMX,1,milliseconds); break;
				case 4: myRGBWLight.allBlueOn(myOpenDMX,1,milliseconds); break; 
				case 5: myRGBWLight.allAmberOn(myOpenDMX,1,milliseconds); break; 
				case 6: myRGBWLight.allWhiteOn(myOpenDMX,1,milliseconds); break; 
				case 7: myRGBWLight.randomPattern(myOpenDMX,1,milliseconds); break; 
				case 8: myRGBWLight.rainbowCrossDissolve(myOpenDMX,1,milliseconds); break;
				case 0: doContinue = false; break; 
				default: std::cout << "Invalid argument. please select again." << std::endl; 
				}

				switch (input){
				case 1: myRGBAWLight.allColorsRampDown(myOpenDMX,1,milliseconds); break;
				case 2: myRGBAWLight.allRedOn(myOpenDMX,1,milliseconds); break; 
				case 3: myRGBAWLight.allGreenOn(myOpenDMX,1,milliseconds); break;
				case 4: myRGBAWLight.allBlueOn(myOpenDMX,1,milliseconds); break; 
				case 5: myRGBAWLight.allAmberOn(myOpenDMX,1,milliseconds); break; 
				case 6: myRGBAWLight.allWhiteOn(myOpenDMX,1,milliseconds); break; 
				case 7: myRGBAWLight.randomPattern(myOpenDMX,1,milliseconds); break; 
				case 8: myRGBAWLight.rainbowCrossDissolve(myOpenDMX,1,milliseconds); break;
				case 0: doContinue = false; break; 
				default: std::cout << "Invalid argument. please select again." << std::endl; 
				}

				switch (input){
				case 1: myLights.allColorsRampDown(myOpenDMX, 1, milliseconds); break;
				case 2: myLights.allOnSingleColor(myOpenDMX, "r", 1, milliseconds); break; 
				case 3: myLights.allOnSingleColor(myOpenDMX, "g", 1, milliseconds); break;
				case 4: myLights.allOnSingleColor(myOpenDMX, "b", 1, milliseconds); break; 
				case 5:	myLights.allOnSingleColor(myOpenDMX, "a", 1, milliseconds); break; 
				case 6: myLights.allOnSingleColor(myOpenDMX, "w", 1, milliseconds); break; 
				case 7: myLights.randomPattern(myOpenDMX,1,milliseconds); break; 
				case 8: myLights.rainbowCrossDissolve(myOpenDMX,1,milliseconds); break;
				case 0: doContinue = false; break; 
				default: std::cout << "Invalid argument. please select again." << std::endl; 
				}			//end switch	
			}		//end while doContinue
			*/
		} //end if 
		else{
			system("pause"); 
			return 1;
		}

	}

	FreeLibrary(dllHandle_); 

	system("pause"); 
	return 0;
}

