#ifndef IMAGECV_H
#define IMAGECV_H
/**
	by: Xavier Guay
	This file is a header for a class that manages an image for computer vision
**/

#include <string>

class ImageCV
{

public:
	ImageCV(int length, int width);
	int setImage(string &imagePath);
	bool isEmpty();
	int getWidth();
	int getLength();
	unsigned char getImageAt(int x, int y);

private:
	bool empty;
	int len;
	int wid;
	unsigned char image[len][wid];
};

#endif 