/** 
 a file just to test stuff
*/

//#define _DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

#include "ImageCV.h"

 int main(int argv, char** argc){
 	std::string fInPath;
	fInPath += argc[1];
	int length = atoi(argc[2]);
	int width = atoi(argc[3]);

	std::string fOutPath;
	fOutPath = fInPath;
	fOutPath += "_test";

	ImageCV image(length, width);
 	//import image
 	image.setImage(fInPath);
 	image.saveImage(fOutPath);


 	return 0;
 }