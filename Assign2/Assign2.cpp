/** ASSIGN2.CPP
	Xavier Guay
	Computer Vision

	Purpose: This program will pass an image through an 2D FFT get the 
	frequencys and display it. Then it will apply a Butter-worth Low Pass
	Filter, and display it. Then it will go through and pass the image
	through the 2D IFFT and redisplay the filtered image
**/

#include <stdlib>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <iostream>

int main(int argv, char** argc)
{
	//The function should pass in an image file a length and a width
	int imagePath = argc[1];
	int length = argc[2];
	int width = arg[3];
}