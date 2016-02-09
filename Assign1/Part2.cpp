/** Part1.cpp
	Computer Vision
	Assignment 1 Part 2
	Xavier Guay
	PUROPSE:
	Open a given image binary. Reduce it's size by half in both length and width
**/

//#define _DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <iostream>

/*This program takes three inputs:
	file - 	A binary image file
	Length- How tall the file is in pixels
	Width- 	How wide the file is in pixels
*/
int main(int argv, char** argc){
	//get inputs
	std::string fInPath;
	fInPath += argc[1];
	int length = atoi(argc[2]);
	int width = atoi(argc[3]);
	
	if(1 == width % 2)
	{
		printf("ERROR: width must be divisible by 2\n");
		return -1;
	}

	std::string fOutPath;
	fOutPath = fInPath;
	fOutPath += "_part2";

#ifdef _DEBUG
	printf("%s\n", fInPath.c_str());
	printf("%s\n", fOutPath.c_str());
	printf("%d\n", length);
	printf("%d\n", width);
#endif


	//open file to copy from
	std::FILE* fIn = std::fopen(fInPath.c_str(),"r"); //open the given file

	//open new file to edit
	std::FILE* fOut = std::fopen(fOutPath.c_str(), "w");

	//fwrite(&buf[0], sizeof(buf[0]), buf.size(), fOut);

	unsigned char tempPixel;
	//loop through the file copying each pixel if nessary
	for(int lCount = 1; lCount <= length/2; lCount++){
		//loop through line of image. because we want to copy the left half
		//we only copy the first half of the line
		#ifdef _DEBUG
			printf("%d: ", lCount);
		#endif
		for(int wCount = 1; wCount <= width; wCount++){
			//read in a pixel to temp pixel
			std::fread(&tempPixel, sizeof(tempPixel), 1, fIn);

			if(1 == wCount%2){
				fwrite(&tempPixel, sizeof(tempPixel), 1, fOut);
			#ifdef _DEBUG
				printf("%c", tempPixel);
			#endif
			}
		}
		//get rid of the next line
		for(int wCount = 1; wCount <= width; wCount++){
			std::fread(&tempPixel, sizeof(tempPixel), 1, fIn);
		}
		#ifdef _DEBUG
			printf("\n");
		#endif
	}

	//close files

	//display new image
	#ifdef _DEBUG
		printf("New file: %s\n", fOutPath.c_str());
		printf("New length: %d\n", length/2);
		printf("New width: %d\n", width/2);	
	#endif

	std::string xvArgs;
	xvArgs.append("(echo P5; echo ");
	xvArgs.append(std::to_string(width/2));
	xvArgs.append(" ");
	xvArgs.append(std::to_string(length/2));
	xvArgs.append("; echo 255; cat ");
	xvArgs.append(fOutPath);
	xvArgs.append(")|xv -&");
	system(xvArgs.c_str());
	//std::cout << xvArgs << '\n';
	//system("(echo P5; echo 128 256; echo 255; cat mri_part1)|xv -&");
	return 0;





	//loop through the file copying each pixel if nessary
	for(int lCount = 0; lCount <= length; lCount += 2){
		for(int wCount = 0; wCount <= width/2; wCount += 2){
			//copy pixel at wCount
			printf("%d ", wCount);
		}
		printf("\n");
	}
}
