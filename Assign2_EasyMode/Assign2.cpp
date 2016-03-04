/** ASSIGN2.CPP
	Xavier Guay
	Computer Vision

	Purpose: This program will pass an image through an 2D FFT get the 
	frequencys and display it. Then it will apply a Butter-worth Low Pass
	Filter, and display it. Then it will go through and pass the image
	through the 2D IFFT and redisplay the filtered image
**/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <iostream>

//#define _DEBUG

void viewImage(std::string path, int length, int width);

int main(int argv, char** argc)
{
	//The function should pass in an image file a length and a width
	std::string fInPath;
	fInPath += argc[1];
	int length = atoi(argc[2]);
	int width = atoi(argc[3]);
	
	if(1 == width % 2)
	{
		printf("ERROR: width must be divisible by 2\n");
		return -1;
	}

	
	//LOAD IMAGE
	unsigned char inImage [length][width];
	std::FILE* fIn = std::fopen(fInPath.c_str(),"r"); //open the given file
	//copy pixels into the image
	for(int lCount = 1; lCount <= length; lCount++){
		unsigned char tempPixel;
		for(int wCount = 1; wCount <= width; wCount++){
			std::fread(&tempPixel, sizeof(tempPixel), 1, fIn);
			inImage[lCount][wCount] = tempPixel;
		}
	}

	#ifdef _DEBUG
	//Save to file to check that loaded correctly
	std::string fLoadedPath;
	fLoadedPath = fInPath;
	fLoadedPath += "_loaded";

	std::FILE* fLoaded = std::fopen(fLoadedPath.c_str(), "w");
	for(int lCount = 1; lCount <= length; lCount++){
		unsigned char tempPixel;
		for(int wCount = 1; wCount <= width; wCount++){
			tempPixel = inImage[lCount][wCount];
			fwrite(&tempPixel, sizeof(tempPixel), 1, fLoaded);
		}
	}
	viewImage(fLoadedPath, length,width);
	
	#endif



	//GET SPECTRUM
	//TODO do spacial transform

	//TODO do 1D FFT by row

	//TODO do 1D FFT by column

	//TODO get spectrum out

	//TODO save spectrum


	//ADD FILTER



	//GET IMAGE BACK
	//TODO do 1D IFFT by column

	//TODO do 1D IFFT by row



}

void viewImage(std::string path, int length, int width)
{
	std::string xvArgs;
	xvArgs.append("(echo P5; echo ");
	xvArgs.append(std::to_string(width));
	xvArgs.append(" ");
	xvArgs.append(std::to_string(length));
	xvArgs.append("; echo 255; cat ");
	xvArgs.append(path);
	xvArgs.append(")|xv -&");
	system(xvArgs.c_str());
}