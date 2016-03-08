/** ASSIGN2.CPP
	Xavier Guay
	Computer Vision

	Purpose: This program will pass an image through an 2D FFT get the 
	frequencys and display it. Then it will apply a Butter-worth Low Pass
	Filter, and display it. Then it will go through and pass the image
	through the 2D IFFT and redisplay the filtered image
**/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <iostream>
#include <limits>

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

//#define _DEBUG
void viewImage(std::string path, int length, int width);
void four1(float data[], int nn, int isign);

int main(int argv, char** argc)
{
	//The function should pass in an image file a length and a width
	std::string fInPath;
	fInPath += argc[1];
	int length = atoi(argc[2]);
	int width = atoi(argc[3]);
	float power = atof(argc[4]);
	
	if(1 == width % 2)
	{
		printf("ERROR: width must be divisible by 2\n");
		return -1;
	}

	
	//LOAD IMAGE
	unsigned char inImage [length][width];
	std::FILE* fIn = std::fopen(fInPath.c_str(),"r"); //open the given file
	//copy pixels into the image
	for(int lCount = 0; lCount < length; lCount++){
		unsigned char tempPixel;
		for(int wCount = 0; wCount < width; wCount++){
			std::fread(&tempPixel, sizeof(tempPixel), 1, fIn);
			inImage[lCount][wCount] = tempPixel;
		}
	}

	#ifdef _DEBUG
	//Save to file to check that loaded correctly
	std::string fLoadedPath;
	fLoadedPath = fInPath;
	fLoadedPath += "_loaded";

	std::FILE* fLoaded = std::fopen(fLoadedPath.c_str(), "w");
	for(int lCount = 0; lCount < length; lCount++){
		unsigned char tempPixel;
		for(int wCount = 0; wCount < width; wCount++){
			tempPixel = inImage[lCount][wCount];
			fwrite(&tempPixel, sizeof(tempPixel), 1, fLoaded);
		}
	}
	viewImage(fLoadedPath, length, width);
	#endif


	float FFTRealImage[length][width];
	float FFTImaginaryImage[length][width];
	float fSpectrum[length][width];
	{
		//GET SPECTRUM
		//do spatial transform
		float transformImage[length][width];
		for(int lCount = 0; lCount < length; lCount++){
			float tempValue;
			for(int wCount = 0; wCount < width; wCount++){
				transformImage[lCount][wCount] = (float) inImage[lCount][wCount];
			}
		}

		
		for(int lCount = 0; lCount < length; lCount++){
			float tempValue;
			for(int wCount = 0; wCount < width; wCount++){
				tempValue = ((float) transformImage[lCount][wCount]) * powf(-1,lCount + wCount);
				transformImage[lCount][wCount] = tempValue;
			}
		}
	
		//Do 1D FFT by row	
		float FFT1Image[length][width*2];
		for(int lCount = 0; lCount < length; lCount++){
			float tempPixelLine[width*2]; //temperaraly stores a pixel line  to feed to 1D FFT
		
			//pad in imaginary numbers
			for(int copyCount = 0; copyCount < width*2; copyCount++)
				tempPixelLine[copyCount] = 0;
			//put in real numbers
			for(int copyCount = 0; copyCount < width*2; copyCount+=2)
				tempPixelLine[copyCount] = transformImage[lCount][copyCount/2];

			//Feed to 1D FFT
			four1(tempPixelLine-1, width, 1);
		
			//Copy into FFT1Image
			for(int wCount = 0; wCount < width*2; wCount++)
				FFT1Image[lCount][wCount] = tempPixelLine[wCount];

		}
	
		//1D FFT by column
		
		for(int wCount = 0; wCount < width; wCount++){
			float tempPixelLine[length*2];//real and imaginary
			for(int count = 0; count < length*2; count++)
				tempPixelLine[count] = 0;

			//copy values into tempPixelLine
			for(int lCopyCount = 0; lCopyCount < length*2; lCopyCount+=2){
				tempPixelLine[lCopyCount]   = FFT1Image[lCopyCount/2][wCount*2];
				tempPixelLine[lCopyCount+1] = FFT1Image[lCopyCount/2][wCount*2+1];
			}
			//put line through 1D FFT
			four1(tempPixelLine-1, length, 1);

			//copy Data into images
			for(int lCopyCount = 0; lCopyCount < length*2; lCopyCount+=2){
				FFTRealImage[lCopyCount/2][wCount]      = tempPixelLine[lCopyCount];
				FFTImaginaryImage[lCopyCount/2][wCount] = tempPixelLine[lCopyCount+1];
			}
		}

		
	}

	//ButterworthFilter
	{
		float cutoff = 2; 
		float n = power;//set between 1 and -1
		float filter = 0;
		float distPixel = 0;
		float bFilter[length][width];
		for(int lCount = 0; lCount < length; lCount++){
			for(int wCount = 0; wCount < width; wCount++){
				bFilter[lCount][wCount] = 1;
			}
		}

		for(int lCount = 0; lCount < length; lCount++){
			for(int wCount = 0; wCount < width; wCount++){
				distPixel = sqrt(pow(lCount-length/2,2)+pow(wCount-width/2,2));
				//distPixel = sqrt(pow(lCount,2)+pow(wCount,2));
				filter = 1/(1+pow(distPixel/cutoff,2*n));
				bFilter[lCount][wCount] *= filter;
			}
		}

		//multiply the filter and array
		for(int lCount = 0; lCount < length; lCount++){
			for(int wCount = 0; wCount < width; wCount++){
				FFTRealImage[lCount][wCount] *= bFilter[lCount][wCount];
				FFTImaginaryImage[lCount][wCount] *= bFilter[lCount][wCount];
			}
		}
	}

	//get spectrum out
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width; wCount++){
			float internal = powf(FFTImaginaryImage[lCount][wCount],2);
			internal += powf(FFTRealImage[lCount][wCount],2);
			fSpectrum[lCount][wCount] = sqrt(internal);
		}
	}
	//Normalize
	unsigned char spectrum[length][width];
	float maxFFT = std::numeric_limits<float>::min();
	float minFFT = std::numeric_limits<float>::max();
	//get max and min
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width; wCount++){
			if(fSpectrum[lCount][wCount] > maxFFT){
				maxFFT = fSpectrum[lCount][wCount];
			}
			if(fSpectrum[lCount][wCount] < minFFT){
				minFFT = fSpectrum[lCount][wCount];
			}
		}
	}

	#ifdef _DEBUG
	printf("Min: %f Max %f\n", minFFT, maxFFT);
	#endif

	//set to normal
	for(int lCount = 0; lCount < length; lCount++){
		float denominator = maxFFT - minFFT;
		for(int wCount = 0; wCount < width; wCount++){
			float numerator = fSpectrum[lCount][wCount]-minFFT;
			spectrum[lCount][wCount] = (unsigned char)(numerator / denominator *255);
		}
	}

	//Save Spectrum to file
	std::string fSpectPath;
	fSpectPath = fInPath;
	fSpectPath += "_spectrum";

	std::FILE* fSpect = std::fopen(fSpectPath.c_str(), "w");
	for(int lCount = 0; lCount < length; lCount++){
		unsigned char tempPixel;
		for(int wCount = 0; wCount < width; wCount++){
			tempPixel = spectrum[lCount][wCount];
			fwrite(&tempPixel, sizeof(tempPixel), 1, fSpect);
		}
	}
	viewImage(fSpectPath, length, width);


	//GET IMAGE BACK
	float IFFTImage[length][width*2];
	//set all to 0
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width*2; wCount++){
			IFFTImage[lCount][wCount] = 0;
		}
	}

	//copy in real and imaginary arrays
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width*2; wCount+=2){
			IFFTImage[lCount][wCount]   = FFTRealImage[lCount][wCount/2];
			IFFTImage[lCount][wCount+1] = FFTImaginaryImage[lCount][wCount/2];
		}
	}

	//Do 1D IFFT by column
	for(int wCount = 0; wCount< width*2; wCount+=2){
		float tempPixelLine[length*2];
		for(int lCount = 0; lCount < length*2; lCount+=2){
			tempPixelLine[lCount] = IFFTImage[lCount/2][wCount];
			tempPixelLine[lCount +1] = IFFTImage[lCount/2][wCount+1];
		}

		//put through IFFT
		four1(tempPixelLine-1, length, -1);
		//put back into array
		for(int lCount = 0; lCount < length*2; lCount+=2){
			IFFTImage[lCount/2][wCount] = tempPixelLine[lCount];
			IFFTImage[lCount/2][wCount+1]= tempPixelLine[lCount+1];
		}
	}

	//Do 1D IFFT by row
	for(int lCount = 0; lCount < length; lCount++){
		float tempPixelLine[width*2];
		//copy in line from image
		for(int wCount = 0; wCount < width*2; wCount++)
			tempPixelLine[wCount]= IFFTImage[lCount][wCount];

		four1(tempPixelLine-1, width, -1);

		//copy back to image
		for(int wCount = 0; wCount< width*2; wCount++)
			IFFTImage[lCount][wCount] = tempPixelLine[wCount];
	}


	//spatial tansform/*
	{
		float realPart[length][width];
		float imagPart[length][width];
		//copy real and imaginary parts into array
		for(int lCount = 0; lCount < length; lCount++){
			for(int wCount = 0; wCount < width; wCount++){
				realPart[lCount][wCount] = IFFTImage[lCount][wCount*2];
				imagPart[lCount][wCount] = IFFTImage[lCount][wCount*2 +1];
			}
		}
		for(int lCount = 0; lCount < length; lCount++){
			for(int wCount = 0; wCount < width; wCount++){
				realPart[lCount][wCount] = realPart[lCount][wCount] * powf(-1,lCount+wCount);
				imagPart[lCount][wCount] = imagPart[lCount][wCount] * powf(-1,lCount+wCount);
			}
		}
		//copy back to IFFTImage
		for(int lCount = 0; lCount < length; lCount++){
			for(int wCount = 0; wCount < width; wCount++){
				IFFTImage[lCount][wCount*2] = realPart[lCount][wCount];
				IFFTImage[lCount][wCount*2 +1] = imagPart[lCount][wCount];
			}
		}
	}/**/


	
	//Square root of sums
	float realIFFT[length][width];
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width; wCount++){
			//printf("%d %d\n", wCount, wCount*2 +1 );
			float internal = powf(IFFTImage[lCount][wCount*2+1],2); //I^2
			internal += powf(IFFTImage[lCount][wCount*2],2); //I^2 + R^2
			realIFFT[lCount][wCount] = sqrt(internal); //sqrt(I^2+R^2)
			//printf("%f %f\n", realIFFT[lCount][wCount], internal);
		}
	}

	//Normalize
	unsigned char normIFFT[length][width];
	float maxIFFT = std::numeric_limits<float>::min();
	float minIFFT = std::numeric_limits<float>::max();
	//get max and min
	for(int lCount = 0; lCount < length; lCount++){
		for(int wCount = 0; wCount < width; wCount++){
			if(realIFFT[lCount][wCount] > maxIFFT){
				maxIFFT = realIFFT[lCount][wCount];
			}
			if(realIFFT[lCount][wCount] < minIFFT){
				minIFFT = realIFFT[lCount][wCount];
			}
		}
	}
	//printf("Min: %f Max: %f\n", minIFFT, maxIFFT);

	for(int lCount = 0; lCount < length; lCount++){
		float denominator = maxIFFT - minIFFT;
		for(int wCount = 0; wCount < width; wCount++){
			float numerator = realIFFT[lCount][wCount]-minIFFT;
			normIFFT[lCount][wCount] = (unsigned char)(numerator / denominator *255);

		}
	}
	//information is now normalized in normIFFT

	//Display
	std::string fIFFTPath;
	fIFFTPath = fInPath;
	fIFFTPath += "_IFFT";

	std::FILE* fIFFT = std::fopen(fIFFTPath.c_str(), "w");
	for(int lCount = 0; lCount < length; lCount++){
		unsigned char tempPixel;
		for(int wCount = 0; wCount < width; wCount++){
			tempPixel = normIFFT[lCount][wCount];
			fwrite(&tempPixel, sizeof(tempPixel), 1, fIFFT);
		}
	}
	viewImage(fIFFTPath, length, width);

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

void four1(float data[], int nn, int isign)
{
	int n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	float tempr,tempi;
    n=nn << 1;
    j=1;
    for (i=1;i<n;i+=2) {
		if (j > i) {
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
        m=n >> 1;
        while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
        j += m;
	}
    mmax=2;
    while (n > mmax) {
    	istep=2*mmax;
        theta=6.28318530717959/(isign*mmax);
        wtemp=sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi=sin(theta);
        wr=1.0;
        wi=0.0;
        for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
                j=i+mmax;
                tempr=wr*data[j]-wi*data[j+1];
                tempi=wr*data[j+1]+wi*data[j];
                data[j]=data[i]-tempr;
                data[j+1]=data[i+1]-tempi;
                data[i] += tempr;
            	data[i+1] += tempi;
			}
            wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}
