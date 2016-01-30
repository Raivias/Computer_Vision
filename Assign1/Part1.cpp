/** Part1.cpp
	Computer Vision
	Assignment 1 Part 1
	Xavier Guay
	PUROPSE:
	Open a given image binary. Save it as a .ps but with the right half of 
	the image removed.
**/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>



/*This program takes three inputs:
	file - 	A binary image file
	Length- How tall the file is in pixels
	Width- 	How wide the file is in pixels

*/
int main(int argv, char** argc){
	//get inputs
	//char string = argc[1];
	int length = atoi(argc[2]);
	int width = atoi(argc[3]);
	
	printf("running\n");

	//open file to copy from
	//open new file to edit

	//loop through the file copying each pixel if nessary
	for(int lCount = 0; lCount <= length; lCount++){
		printf("%d: ", lCount);
		/*loop through line of image. because we want to copy the left half
		we only copy the first half of the line*/
		for(int wCount = 0; wCount <= width/2; wCount++){
			//copy pixel at wCount
			printf("%d ", wCount);
		}
		printf("\n");
	}
	
	//close files	

	printf("ran\n");	
	return 1;
}
