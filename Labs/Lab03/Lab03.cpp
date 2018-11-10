#include "Blur.h"
#include "EdgeDetector.h"

#include<string>
#include<iostream>


int main(int argc, char * argv[])
{
	Blur blurImg; // Khởi tạo lớp blurimage
	EdgeDetector ED; // Khởi tạo lớp ED
	
	Mat srcImage, dstImage; 
	string Command(argv[1]); //
	
	
	int result = 0;

	srcImage = imread(argv[2], -1);

	if (Command == "--mean")
	{
		int kW = atoi(argv[3]), kH = atoi(argv[4]);
		result = blurImg.BlurImage(srcImage, dstImage, kW, kH, 0);
	}
	else if (Command == "--median")
	{
		int kW = atoi(argv[3]), kH = atoi(argv[4]);
		result = blurImg.BlurImage(srcImage, dstImage, kW, kH, 1);
	}
	else if (Command == "--gauss")
	{
		int kW = atoi(argv[3]), kH = atoi(argv[4]);
		result = blurImg.BlurImage(srcImage, dstImage, kW, kH, 2);
	}
	else if (Command == "--sobel")
	{
		result = ED.DetectEdge(srcImage, dstImage, 3, 3, 1); // kWidth = kHeight = 3
	}
	else if (Command == "--prewitt")
	{
		result = ED.DetectEdge(srcImage, dstImage, 3, 3, 2); // kWidth = kHeight = 3
	}
	else if (Command == "--laplace")
	{
		result = ED.DetectEdge(srcImage, dstImage, 3, 3, 3); // kWidth = kHeight = 3
	}
	Mat sobel, laplace, median, gauss;
	//Sobel(srcImage, sobel,srcImage.depth(), 1, 1, 3);
	//Laplacian(srcImage, laplace, srcImage.depth(), 1);
	GaussianBlur(srcImage, gauss, Size(3, 3), 0.5f);
	imshow("Source Image", srcImage);
	imshow("Gauss", gauss);
	//imshow("Sobel", sobel);
	//imshow("Laplace", laplace);
	if (result)
	{
		imshow("Destination Image", dstImage);
	}
	waitKey(0);
	
	return 0;
}