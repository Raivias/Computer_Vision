#include "Cake.h"

#include <exception>
#include <iostream>
#include <math.h>
#include <string>

int importImage(std::string fPath, unsigned char **image, int length, int width);
int saveImage(std::string fPath, unsigned char **image, int length, int width);
void viewImage(std::string path, int length, int width);
void copyImage(unsigned char **orig, int length, int width, unsigned char **copy);
void AddImages(unsigned char **output, unsigned char **input, int length, int width);
void HorizSobel(unsigned char **image, int length, int width);
void VertiSobel(unsigned char **image, int length, int width);



int main(int argv, char** argc)
{
	std::string fInPath;
	fInPath += argc[1];
	int length = atoi(argc[2]);
	int width = atoi(argc[3]);

	unsigned char **origImage = (unsigned char **)malloc(sizeof(unsigned char *)*length);
	for(int i = 0; i<length; i++)
	{
		origImage[i] = (unsigned char *)malloc(sizeof(unsigned char)*width);
		for(int j = 0; j<width; j++)
			origImage[i][j] = 0;
	}

	//Import Image
	importImage(fInPath, origImage, length, width);
	printf("Importing image\n");

	//Copy Image for Horz and Vertical Sobels
	unsigned char **horizImage = (unsigned char **)malloc(sizeof(unsigned char *)*length);
	for(int i = 0; i<length; i++)
	{
		horizImage[i] = (unsigned char *)malloc(sizeof(unsigned char)*width);
		for(int j = 0; j<width; j++)
			horizImage[i][j] = 0;
	}
	
	unsigned char **vertiImage = (unsigned char **)malloc(sizeof(unsigned char *)*length);
	for(int i = 0; i<length; i++)
	{
		vertiImage[i] = (unsigned char *)malloc(sizeof(unsigned char)*width);
		for(int j = 0; j<width; j++)
			vertiImage[i][j] = 0;
	}

	printf("coping images to horizImage and vertiImage\n");
	copyImage(origImage, length, width, horizImage);
	copyImage(origImage, length, width, vertiImage);

	printf("getting horiz\n");
	//Apply Horiz Sobel
	HorizSobel(horizImage, length, width);
	std::string horizPath;
	horizPath += fInPath;
	horizPath += "_Horiz";

	saveImage(horizPath, horizImage, length, width);
	viewImage(horizPath, length, width);
	
	printf("getting verti\n");
	//Apply Vertical Sobel
	VertiSobel(vertiImage, length, width);
	std::string vertiPath;
	vertiPath += fInPath;
	vertiPath += "_Verti";

	saveImage(vertiPath, vertiImage, length, width);
	viewImage(vertiPath, length, width);
	
	//Add together
	unsigned char **sobelImage = (unsigned char **)malloc(sizeof(unsigned char *)*length);
	for(int i = 0; i<length; i++)
	{
		sobelImage[i] = (unsigned char *)malloc(sizeof(unsigned char)*width);
		for(int j = 0; j<width; j++)
			sobelImage[i][j] = 0;
	}

	AddImages(sobelImage, horizImage, length, width);
	AddImages(sobelImage, vertiImage, length, width);

	//Save and View Image
	std::string fOutPath;
	fOutPath += fInPath;
	fOutPath += "_Sobel";

	saveImage(fOutPath, sobelImage, length, width);
	viewImage(fOutPath, length, width);
	/**/

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

int saveImage(std::string fPath, unsigned char **image, int length, int width)
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
	return 0;
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

void copyImage(unsigned char **orig, int length, int width, unsigned char **copy)
{
	for(int lCount = 0; lCount < length; lCount++)
	{
		for(int wCount = 0; wCount < width; wCount++)
			copy[lCount][wCount] = orig[lCount][wCount];
	}
}

void AddImages(unsigned char **output, unsigned char **input, int length, int width)
{
	for(int lCount = 0; lCount < length; lCount++)
	{
		for(int wCount = 0; wCount < width; wCount++)
			output[lCount][wCount] += input[lCount][wCount];
	}
}

void HorizSobel(unsigned char **image, int length, int width)
{
	printf("In Horizontal\n");
	//Copy Image for Horz and Vertical Sobels
	unsigned char **tempImage = (unsigned char **)malloc(sizeof(unsigned char *)*length);
	for(int i = 0; i<length; i++)
	{
		tempImage[i] = (unsigned char *)malloc(sizeof(unsigned char)*width);
		for(int j = 0; j<width; j++)
			tempImage[i][j] = 0;
	}

	int weights[3][3];
	weights[0][0] = -1;// |				|
	weights[0][1] = -2;// |	-1	-2	-1	|
	weights[0][2] = -1;// |				|
	weights[1][0] = 0; // |	0	0	0	|
	weights[1][1] = 0; // |				|
	weights[1][2] = 0; // |	1	2	1	|
	weights[2][0] = 1; // |				|
	weights[2][1] = 2; //  
	weights[2][2] = 1; // 
	for(int lCount = 0; lCount < length; lCount++)
	{
		for(int wCount = 0; wCount<width; wCount++)
		{
			//get pointf for sobel
			unsigned char frame[3][3];
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
					frame[i][j] = 0; //pad frame
			}

			for(int i=-1; i<=1; i++)
			{
				printf("i: %d\n", i);
				if(lCount+i >= 0 && lCount+i < length)
				{
					for(int j=-1; j<=1; j++)
					{
						if(wCount+j >= 0 && wCount+j < width)
						{
							frame[i+1][j+1]=image[lCount+i][wCount+j];
							printf("i:\t%d j:\t%d l:\t%d w:\t%d\n", i, j, lCount, wCount);
						}
					}
				}
			}
			int sum = 0;
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
				{
					sum += weights[i][j]*frame[i][j];
				}
			}

			tempImage[lCount][wCount] = (unsigned char) sum/9;
		}
	}

	copyImage(tempImage, length, width, image);
}

void VertiSobel(unsigned char **image, int length, int width)
{
	printf("In Vertical\n");
	//Copy Image for Horz and Vertical Sobels
	unsigned char **tempImage = (unsigned char **)malloc(sizeof(unsigned char *)*length);
	for(int i = 0; i<length; i++)
	{
		tempImage[i] = (unsigned char *)malloc(sizeof(unsigned char)*width);
		for(int j = 0; j<width; j++)
			tempImage[i][j] = 0;
	}

	int weights[3][3];
	weights[0][0] = -1;// |				|
	weights[0][1] = 0; // |	-1	0	1	|
	weights[0][2] = 1; // |				|
	weights[1][0] = -2;// |	-2	0	2	|
	weights[1][1] = 0; // |				|
	weights[1][2] = 2; // |	-1	0	1	|
	weights[2][0] = -1;// |				|
	weights[2][1] = 0; //  
	weights[2][2] = 1; // 

	for(int lCount = 0; lCount < length; lCount++)
	{
		for(int wCount = 0; wCount<width; wCount++)
		{

			//get pointf for sobel
			unsigned char frame[3][3];
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
					frame[i][j] = 0; //pad frame
			}

			for(int i=-1; i<=1; i++)
			{
				printf("i: %d\n", i);
				if(lCount+i >= 0 && lCount+i < length)
				{
					for(int j=-1; j<=1; j++)
					{
						if(wCount+j >= 0 && wCount+j < width)
						{
							frame[i+1][j+1]=image[lCount+i][wCount+j];
							printf("i:\t%d j:\t%d l:\t%d w:\t%d\n", i, j, lCount, wCount);
						}
					}
				}
			}
			
			int sum = 0;
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
				{
					sum += weights[i][j]*frame[i][j];
				}
			}
		
			tempImage[lCount][wCount] = (unsigned char) sum/9;
		}
	}

	copyImage(tempImage, length, width, image);
}