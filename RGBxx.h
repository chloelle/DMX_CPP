//Class definition for RGBWxx triplet/quartet/quintuple ... 

#ifndef RGBxx_H
#define RGBxx_H

class RGBxx{
public: int R;
		int G; 
		int B; 
		int W; 
		int A;
		int numChannels; 
public: 
	RGBxx(int numChannels_, int r, int g, int b, int a, int w);
	friend RGBxx operator+(const RGBxx &obj1, const RGBxx &obj2);
	friend RGBxx operator-(const RGBxx &obj1, const RGBxx &obj2);
	friend RGBxx scalarMultiply(const RGBxx &obj1, float scalar); 
};

#endif