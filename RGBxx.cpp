//Class implementation  for RGBWxx triplet/quartet/quintuple ... 

#include "stdafx.h"
#include "RGBxx.h"

RGBxx::RGBxx(int numChannels_, int r, int g, int b, int a, int w){
	numChannels = numChannels_; 
	R = r; 
	G = g; 
	B = b; 
	if (numChannels > 3){
		A = a; 
	}
	else {
		A = 0; 
	}
	if (numChannels > 4){
		W = w; 
	}
	else{
		W = 0; 
	}
}

RGBxx operator+(const RGBxx &obj1, const RGBxx &obj2){
	if (obj1.numChannels > obj2.numChannels){
		return RGBxx(obj1.numChannels, obj1.R + obj2.R, obj1.G + obj2.G, obj1.B + obj2.B, obj1.A + obj2.A, obj1.W + obj2.W);
	}
	else{
		return RGBxx(obj2.numChannels, obj1.R + obj2.R, obj1.G + obj2.G, obj1.B + obj2.B, obj1.A + obj2.A, obj1.W + obj2.W);
	}
}

RGBxx operator-(const RGBxx &obj1, const RGBxx &obj2){
	if (obj1.numChannels > obj2.numChannels){
		return RGBxx(obj1.numChannels, obj1.R - obj2.R, obj1.G - obj2.G, obj1.B - obj2.B, obj1.A - obj2.A, obj1.W - obj2.W);
	}
	else{
		return RGBxx(obj2.numChannels, obj1.R - obj2.R, obj1.G - obj2.G, obj1.B - obj2.B, obj1.A - obj2.A, obj1.W - obj2.W);
	}
}

RGBxx scalarMultiply(const RGBxx &obj1,float scalar){
	return RGBxx(obj1.numChannels, int(scalar*float(obj1.R)), int(scalar*float(obj1.G)), int(scalar*float(obj1.B)), int(scalar*float(obj1.A)), int(scalar*float(obj1.W)));
}

