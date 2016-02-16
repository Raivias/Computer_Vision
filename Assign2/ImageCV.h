#ifndef IMAGECV_H
#define IMAGECV_H
/**
	by: Xavier Guay
	This file is a header for a class that manages an image for computer vision

	TODO
		add displayImage()
		overload=
**/

#include <string>

class ImageCV
{

public:
	ImageCV();
	ImageCV(int length, int width); //constructor
	ImageCV(ImageCV &copyImage); //TODO: copier constructor
	//~ImageCV(); //deconstructor

	//Modifiers
	int setImage(std::string &imagePath);
	bool saveImage(std::string &imagePath);
	unsigned char getImageAt(int x, int y);

	//Accessor Functions
	bool isEmpty(){return empty;}
	int getLength(){return len;}
	int getWidth(){return wid;}
	
	//Image Modifiers
	//ImageCV centerSpectrum();

	//ImageCV& operator=(ImageCV& oldImage);

private:
	bool empty;
	int len;
	int wid;
	unsigned char** image; //this will be the 2D array of pixels

	bool setImageAt(int x, int y, unsigned char pixel);
	void setEmpty(bool newEmpty){empty = newEmpty;}
	int minPixelValue();
	int maxPixelValue();
};

#endif 