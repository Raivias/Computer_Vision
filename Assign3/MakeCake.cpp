#include "Cake.h"

#include <exception>
#include <iostream>
#include <limits>
#include <math.h>
#include <string>

const int X1  = 256/2 -1;
const int Y1  = 256/2 -1;
const int R1  = 50;
const int I1 = 50; //intensity

const int X2  = 256/2 -1;
const int Y2  = 256/2 -1;
const int R2  = 25;
const int I2 = 50; //intensity

const int X3  = 256/2 -1;
const int Y3  = 256/2 -1;
const int R3  = 10;
const int I3 = 50; //intensity

int importImage(std::string fPath, unsigned char **image, int length, int width);
void saveImage(std::string fPath, unsigned char **image, int length, int width);
void viewImage(std::string path, int length, int width);
void NormalizeImage(unsigned char **imageUC, float **imageF, int length, int width);
void AddUniformNoise(float** image, int length, int width, float nMax);

int main(int argv, char** argc)
{
	std::string fPath;
	fPath += argc[1];
	int length = atoi(argc[2]);
	int width = atoi(argc[3]);
	int noiseLevel = atoi(argc[4]);

	unsigned char **image = (unsigned char **)malloc(sizeof(unsigned char *)*length);
	for(int i = 0; i<length; i++)
	{
		image[i] = (unsigned char *)malloc(sizeof(unsigned char)*width);
		for(int j = 0; j<width; j++)
			image[i][j] = 0;
	}

	float **imageF = (float **)malloc(sizeof(float *)*length);
	for(int i = 0; i<length; i++)
	{
		imageF[i] = (float *)malloc(sizeof(float)*width);
		for(int j = 0; j<width; j++)
			imageF[i][j] = 0;
	}

	
	//add circles
	AddCircle(imageF, length, width, X1, Y1, R1, I1);
	AddCircle(imageF, length, width, X2, Y2, R2, I2);
	AddCircle(imageF, length, width, X3, Y3, R3, I3);

	//add noise
	AddUniformNoise(imageF, length, width, noiseLevel);

	//conver float to unsigned
	NormalizeImage(image, imageF, length, width);

	//save and view file
	saveImage(fPath, image, length, width);
	viewImage(fPath, length, width);
}

int importImage(std::string fPath, unsigned char **image, int length, int width)
{
	std::FILE* fIn = std::fopen(fPath.c_str(), "r");
	//copy pixels into the image
	for(int lCount = 0; lCount < length; lCount++){
		unsigned char tempPixel;
		for(int wCount = 0; wCount < width; wCount++){
			std::fread(&tempPixel, sizeof(tempPixel), 1, fIn);
			image[lCount][wCount] = tempPixel;
		}
	}
	return 0;
}

void saveImage(std::string fPath, unsigned char **image, int length, int width)
{
	std::FILE* fOut = std::fopen(fPath.c_str(), "w");
	for(int lCount = 0; lCount < length; lCount++)
	{
		unsigned char tempPixel;
		for(int wCount = 0; wCount < width; wCount++)
		{
			tempPixel = image[lCount][wCount];
			fwrite(&tempPixel, sizeof(tempPixel), 1, fOut);
		}
	}
	return;
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
	return;
}

void NormalizeImage(unsigned char **imageUC, float **imageF, int length, int width)
{
	float max = std::numeric_limits<float>::min();
	float min = std::numeric_limits<float>::max();
	//get max and min
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width; wCount++){
			if(imageF[lCount][wCount] > max){
				max = imageF[lCount][wCount];
			}
			if(imageF[lCount][wCount] < min){
				min = imageF[lCount][wCount];
			}
		}
	}
	
	//normalize
	for(int lCount = 0; lCount < length; lCount++){
		float denominator = max - min;
		for(int wCount = 0; wCount < width; wCount++){
			float numerator = imageF[lCount][wCount]-min;
			imageUC[lCount][wCount] = (unsigned char)(numerator / denominator *255);
		}
	}
}


void AddUniformNoise(float** image, int length, int width, float nMax){
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width; wCount++){
			//generate noise value

			float k = 2*std::rand()-RAND_MAX;
			k /= RAND_MAX;
			float noise = k*nMax;
			//add to image
			image[lCount][wCount] += noise;
		}
	}
}
