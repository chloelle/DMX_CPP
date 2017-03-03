#include "stdafx.h"
#include "OpenDMX.h"
#include <iostream>

//define the functions we need to send DMX commands, these will be loaded at runtime from ftd2xx.dll 
typedef FT_STATUS (CALLBACK* FT_Open_Type)(int deviceNumber, FT_HANDLE *handle);
typedef FT_STATUS (CALLBACK* FT_ResetDevice_Type)(FT_HANDLE handle); 
typedef FT_STATUS (CALLBACK* FT_SetDivisor_Type)(FT_HANDLE handle, unsigned short usDivisor);
typedef FT_STATUS (CALLBACK* FT_SetDataCharacteristics_Type)(FT_HANDLE handle, unsigned char word_length, unsigned char stop_bits, unsigned char parity); 
typedef FT_STATUS (CALLBACK* FT_SetFlowControl_Type)(FT_HANDLE handle, unsigned short flow_none, unsigned char XonChar, unsigned char XoffChar);
typedef FT_STATUS (CALLBACK* FT_ClrRts_Type)(FT_HANDLE handle); 
typedef FT_STATUS (CALLBACK* FT_Purge_Type)(FT_HANDLE handle, unsigned long mask);
typedef FT_STATUS (CALLBACK* FT_SetBreakOn_Type)(FT_HANDLE handle); 
typedef FT_STATUS (CALLBACK* FT_SetBreakOff_Type)(FT_HANDLE handle); 
typedef FT_STATUS (CALLBACK* FT_Write_Type)(FT_HANDLE handle, BYTE* buffer, DWORD bufferSize, DWORD *ptr_BytesWritten);

//constructor
OpenDMX::OpenDMX(bool isVerbose){
	bufferLength = 513; //dmx 512
	handle = 0;
	done = false; 
	connected = false; 
	bytesWritten = 0; 
	status = FT_OK;

	dllHandle = NULL;
	bits_8 = 8; //dmx
	stop_bits_2 = 2; //dmx
	parity_none = 0; //dmx
	flow_none = 0; //dmx
	purge_rx = 1; 
	purge_tx = 1; 
	verbose = isVerbose;
}

//Start: Open FTDI device 
bool OpenDMX::start(){
	bool runTimeLinkSuccess = FALSE; 
	FT_STATUS res;
	//find the function FT_Open
	FT_Open_Type FT_Open_Ptr = NULL;
	FT_Open_Ptr = (FT_Open_Type)GetProcAddress(dllHandle, "FT_Open");

	if (runTimeLinkSuccess = (NULL != FT_Open_Ptr)) {
		res = FT_Open_Ptr(0, &handle);
		if (verbose){
			std::cout<<"Opening DMX..."<< std::endl; 
			printErrorCode();
		}
		status = res; 
		return false; 
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_Open" << std::endl;
		}
		return true; 
	}	
}

//clears buffer array (host)
void OpenDMX::zerosDMXValue(){ 

	for (int i = 0; i < bufferLength; i++){
		buffer[i] = 0; 
	}
}

//sets buffer array (host)
void OpenDMX::setDMXValue(int channel, unsigned char value){

	if (channel > 0 && channel <= bufferLength && value >= 0 && value <= 255){ //bounds checking for dmx 512
		buffer[channel] = BYTE(value);
	}
	else{
		if (verbose){
			std::cout << "Illegal arguments for DMX input" << std::endl; 
		}
	}
}

//helper function writes buffer array (device) 
//if we don't care about bytes written? [TO-DO] maybe merge with write()
void OpenDMX::writeData(){

	//write some bytes
	bytesWritten = write();

}

//writes buffer array (device)
int OpenDMX::write(){
	//write some bytes
	FT_STATUS res = NULL; 
	bool runTimeLinkSuccess = FALSE; 
	FT_Write_Type FT_Write_Ptr = NULL; 
	FT_Write_Ptr = (FT_Write_Type)GetProcAddress(dllHandle, "FT_Write"); 
	if (runTimeLinkSuccess = (NULL != FT_Write_Ptr)) {
		res = FT_Write_Ptr(handle, buffer, DWORD(bufferLength), &bytesWritten );
		if (verbose){
			std::cout<< "Writing " << bytesWritten << "bytes." << std::endl; 
			printErrorCode();
		}
		return false; 
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_Write" << std::endl;
		}
		return true; 
	}
}

//adapted from C# code
bool OpenDMX::initOpenDMX(){

	FT_STATUS res = NULL; 

	//RESET DEVICE
	bool runTimeLinkSuccess = FALSE; 
	FT_ResetDevice_Type FT_ResetDevice_Ptr = NULL;
	FT_ResetDevice_Ptr = (FT_ResetDevice_Type)GetProcAddress(dllHandle, "FT_ResetDevice");

	if (runTimeLinkSuccess = (NULL != FT_ResetDevice_Ptr)) {
		res = FT_ResetDevice_Ptr(handle);
		if (verbose){
			std::cout<<"Resetting device..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_ResetDevice" << std::endl;
		}
		return true; 
	}

	//SET DIVISOR / baud rate
	runTimeLinkSuccess = FALSE;
	FT_SetDivisor_Type FT_SetDivisor_Ptr = NULL;
	FT_SetDivisor_Ptr = (FT_SetDivisor_Type)GetProcAddress(dllHandle, "FT_SetDivisor");

	if (runTimeLinkSuccess = (NULL != FT_SetDivisor_Ptr)) {
		res = FT_SetDivisor_Ptr(handle, unsigned short(12));
		if (verbose){
			std::cout<<"Setting baud rate..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_SetDivisor" << std::endl;
		}
		return true; 
	}

	//SET DATA CHARACTERISTICS
	runTimeLinkSuccess = FALSE;
	FT_SetDataCharacteristics_Type FT_SetDataCharacteristics_Ptr = NULL; 
	FT_SetDataCharacteristics_Ptr = (FT_SetDataCharacteristics_Type)GetProcAddress(dllHandle, "FT_SetDataCharacteristics");

	if (runTimeLinkSuccess = (NULL != FT_SetDataCharacteristics_Ptr)) {
		res = FT_SetDataCharacteristics_Ptr(handle, bits_8, stop_bits_2, parity_none);
		if (verbose){
			std::cout<<"Setting data characteristics..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_SetDataCharacteristics" << std::endl;
		}
		return true; 
	}

	//SET FLOW CONTROL
	runTimeLinkSuccess = FALSE;
	FT_SetFlowControl_Type FT_SetFlowControl_Ptr = NULL; 
	FT_SetFlowControl_Ptr = (FT_SetFlowControl_Type)GetProcAddress(dllHandle, "FT_SetFlowControl");

	if (runTimeLinkSuccess = (NULL != FT_SetFlowControl_Ptr)) {
		res = FT_SetFlowControl_Ptr(handle,flow_none,0,0);
		if (verbose){
			std::cout<<"Setting flow control..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_SetFlowControl" << std::endl;
		}
		return true; 
	}

	//Clear Request To Send control signal
	runTimeLinkSuccess = FALSE;
	FT_ClrRts_Type FT_ClrRts_Ptr = NULL;
	FT_ClrRts_Ptr = (FT_ClrRts_Type)GetProcAddress(dllHandle, "FT_ClrRts");
	if (runTimeLinkSuccess = (NULL != FT_ClrRts_Ptr)) {
		res = FT_ClrRts_Ptr(handle);
		if (verbose){
			std::cout<<"Clearing request to send control signal..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_ClrRts" << std::endl;
		}
		return true; 
	}

	//Purge receive and transmit buffers in the device
	runTimeLinkSuccess = FALSE;
	FT_Purge_Type FT_Purge_Ptr = NULL; 
	FT_Purge_Ptr = (FT_Purge_Type)GetProcAddress(dllHandle, "FT_Purge"); 
	if (runTimeLinkSuccess = (NULL != FT_Purge_Ptr)) {
		res = FT_Purge_Ptr(handle, purge_tx);
		if (verbose){
			std::cout<< "Purging transmit buffer..."<< std::endl; 
			printErrorCode();
		}
		res = FT_Purge_Ptr(handle, purge_rx);
		if (verbose){
			std::cout<< "Purging receive buffer..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_Purge" << std::endl;
		}
		return true; 
	}
	if (res != NULL){
		status = res; 
	}

	//Sets the BREAK condition for the device.
	runTimeLinkSuccess = FALSE; 
	FT_SetBreakOn_Type FT_SetBreakOn_Ptr = NULL; 
	FT_SetBreakOn_Ptr = (FT_SetBreakOn_Type)GetProcAddress(dllHandle, "FT_SetBreakOn"); 
	if (runTimeLinkSuccess = (NULL != FT_SetBreakOn_Ptr)) {
		res = FT_SetBreakOn_Ptr(handle);
		if (verbose){
			std::cout<<"Setting break condition..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_SetBreakOn" << std::endl;
		}
		return true; 
	}

	//Resets the BREAK condition for the device.
	runTimeLinkSuccess = FALSE; 
	FT_SetBreakOff_Type FT_SetBreakOff_Ptr = NULL; 
	FT_SetBreakOff_Ptr = (FT_SetBreakOff_Type)GetProcAddress(dllHandle, "FT_SetBreakOff"); 
	if (runTimeLinkSuccess = (NULL != FT_SetBreakOff_Ptr)) {
		res = FT_SetBreakOff_Ptr(handle);
		if (verbose){
			std::cout<<"Resetting break condition..."<< std::endl; 
			printErrorCode();
		}
	}
	else{
		if (verbose){
			std::cout << "Could not locate the function: " <<  "FT_SetBreakOff" << std::endl;
		}
		return true; 
	}

	return false;
}

//returns OpenDMX error code based on FTD2XX.h enum
void OpenDMX::printErrorCode(){
	switch(status){
	case 0: std::cout << "FT_OK" << std::endl; break; 
	case 1: std::cout << "FT_INVALID_HANDLE" << std::endl; break; 		
	case 2: std::cout << "FT_DEVICE_NOT_FOUND" << std::endl; break; 
	case 3: std::cout << "FT_DEVICE_NOT_OPENED" << std::endl; break; 
	case 4: std::cout << "FT_IO_ERROR" << std::endl; break; 
	case 5: std::cout << "FT_INSUFFICIENT_RESOURCES" << std::endl; break; 
	case 6: std::cout << "FT_INVALID_PARAMETER" << std::endl; break; 
	case 7: std::cout << "FT_INVALID_BAUD_RATE" << std::endl; break; 
	case 8: std::cout << "FT_DEVICE_NOT_OPENED_FOR_ERASE" << std::endl; break; 
	case 9: std::cout << "FT_DEVICE_NOT_OPENED_FOR_WRITE" << std::endl; break; 
	case 10: std::cout << "FT_FAILED_TO_WRITE_DEVICE" << std::endl; break; 
	case 11: std::cout << "FT_EEPROM_READ_FAILED" << std::endl; break; 
	case 12: std::cout << "FT_EEPROM_WRITE_FAILED" << std::endl; break; 
	case 13: std::cout << "FT_EEPROM_ERASE_FAILED" << std::endl; break; 
	case 14: std::cout << "FT_EEPROM_NOT_PRESENT" << std::endl; break; 
	case 15: std::cout << "FT_EEPROM_NOT_PROGRAMMED" << std::endl; break; 
	case 16: std::cout << "FT_INVALID_ARGS" << std::endl; break; 
	case 17: std::cout << "FT_NOT_SUPPORTED" << std::endl; break; 
	case 18: std::cout << "FT_OTHER_ERROR" << std::endl; break; 
	}
}