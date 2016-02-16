/** IMAGECV_CPP
by : Xavier Guay

PURPOSE: this file is used as to hold and manipulate an image for computer vision


TODO
	Write in throwables for passing bad pixels
	figure out const methods

*/

#include "ImageCV.h"

#include <iostream>
#include <exception>
#include <math.h>

/*  Public Functions  */


ImageCV::ImageCV()
{
	len = 0;
	wid = 0;
	empty = true;
}

ImageCV::ImageCV(int length, int width)
{
	len = length;
	wid = width;
	image = new unsigned char*[len];
	for (int xCount = 0; xCount < len; xCount++)
		image[xCount] = new unsigned char [wid];
	empty = true;
}

ImageCV::ImageCV(ImageCV& copyImage)
{
	//set length and width
	len = copyImage.getLength();
	wid = copyImage.getWidth();
	//initilize image space
	for (int xCount = 0; xCount < len; xCount++)
		image[xCount] = new unsigned char [wid];

	//go through and copy each pixel
	for(int xCount = 0; xCount < len; xCount++)
	{
		for(int yCount = 0; yCount < wid; yCount++)
		{
			//copy pixel over
			setImageAt(xCount, yCount, copyImage.getImageAt(xCount, yCount));
		}
	}
	//set image data
	empty = false;
}

int ImageCV::setImage(std::string &imagePath)
{
	unsigned char tempPixel;
	try
	{
		//open file
		std::FILE* fIn = std::fopen(imagePath.c_str(), "r");
		//copy in the image pixel by pixel
		for(int xCount = 0; xCount < len ; xCount++)
		{
			for(int yCount = 0; yCount < wid; yCount++)
			{
				std::fread(&tempPixel, sizeof(tempPixel), 1, fIn);
				if (setImageAt(xCount, yCount, tempPixel) == false)
					return false;
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception: " << e.what() < "\n";
		return false;
	}

	empty = false;
	return true;
}

bool ImageCV::saveImage(std::string &imagePath)
{
	unsigned char tempPixel;
	try
	{
		std::FILE* fOut = std::fopen(imagePath.c_str(), "w");
		for(int xCount = 0; xCount < len; xCount++)
		{
			for(int yCount = 0; yCount < wid; yCount++)
			{
				tempPixel = getImageAt(xCount, yCount);
				fwrite(&tempPixel, sizeof(tempPixel), 1, fOut);
			}
		}

	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception: " << e.what() < "\n";
		return false;
	}
	
	return true;
}

unsigned char ImageCV::getImageAt(int x, int y)
{
	//make checks to ensure data exists
	if (isEmpty())
	{
		std::cerr << "ERROR: ImageCV is empty!\n";
		return 0;
	}

	if(x >= len || y >= wid)
	{
		std::cerr << "ERROR: requested point is out of bounds!\n";
		return 0;
	}

	return image[x][y];
}

/*
ImageCV ImageCV::centerSpectrum()
{
	ImageCV retImage(); //Check this line
	unsigned char tempPixel;
	for (int xCount = 0; xCount < len; xCount++)
	{
		for(int yCount = 0; yCount < wid; yCount++)
		{
			tempPixel = getImageAt(xCount, yCount);
			tempPixel = tempPixel 
			//set temp pixel in new image
		}
	}

}*/

/*
ImageCV& ImageCV::operator=(ImageCV& oldImage)
{
	ImageCV retImage = new ImageCV(oldImage);
	return retImage;
	//return new ImageCV(oldImage);
}*/

/*  Private Functions  */
bool ImageCV::setImageAt(int x, int y, unsigned char pixel)
{
	//make checks to ensure data exists
	if(x > len || y > wid)
	{
		std::cerr << "ERROR: requested point is out of bounds!\n";
		return false;
	}

	image[x][y] = pixel;
	return true;
}

int ImageCV::minPixelValue(){
	int minValue = 10000;//TODO change to infinity
	unsigned char tempPixel;
	for(int xCount = 0; xCount < len; xCount++)
	{
		for (int yCount = 0; yCount < wid; yCount++)
		{
			tempPixel = getImageAt(xCount, yCount);
			if (tempPixel < minValue)
				minValue = tempPixel;
		}
	}

	return minValue;
}

int ImageCV::maxPixelValue(){
	int maxValue = -1;//TODO change to infinity
	unsigned char tempPixel;
	for(int xCount = 0; xCount < len; xCount++)
	{
		for (int yCount = 0; yCount < wid; yCount++)
		{
			tempPixel = getImageAt(xCount, yCount);
			if (tempPixel > maxValue)
				maxValue = tempPixel;
		}
	}

	return maxValue;
}