#include "Noise.h"

void AddUniformNoise(float** image, int length, int width, int nMax){
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width; wCount++){
			//generate noise value
			int k = (2*rand() -RAND_MAX) /RAND_MAX;
			int noise = k*nMax;
			//add to image
			image[lCount][wCount] += noise;
		}
	}
}