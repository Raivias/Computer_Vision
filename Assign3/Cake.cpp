#include <math.h>
#include "Cake.h"


void AddCircle(float** image, int length, int width, int centerX, int centerY, int radius, int amp){
	for(int lCount; lCount < length; lCount++){
		for(int wCount; wCount < width; wCount++){
			int tempX = lCount - centerX;
			int tempY = wCount - centerY;
			if(pow(tempX,2) + pow(tempY,2) - pow(radius, 2) <= 0)
				image[lCount][wCount] += amp;
		}
	}
}
