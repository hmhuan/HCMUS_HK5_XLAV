#include "ColorTransformer.h"
#include "Converter.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char * argv[]) {
	
	Mat srcImg, destImg;
	Mat hsv;
	int result;
	Converter cv;
	ColorTransformer colorTf;
	
	string Command = string(argv[1]);
	srcImg = imread(argv[2]);
	cout << srcImg.type() << endl;
	if (Command == "--rgb2gray") {
		result = cv.Convert(srcImg, destImg, 0);
	}
	else if (Command == "--gray2rgb") {
		result = cv.Convert(srcImg, destImg, 1);
	}
	else if (Command == "--rgb2hsv") {
		result = cv.Convert(srcImg, destImg, 2);
	}
	else if (Command == "--hsv2rgb") {
		result = cv.Convert(srcImg, destImg, 3);
	}
	else if (Command == "--bright") {
		uchar b = atoi(argv[3]);
		result = colorTf.ChangeBrightness(srcImg, destImg, b);
	}
	else if (Command == "--contrast") {
		float c = atof(argv[3]);
		result = colorTf.ChangeContrast(srcImg, destImg, c);
	}
	else if (Command == "--hist") {
		result = colorTf.CalcHistogram(srcImg, destImg);
	}
	else if (Command == "--equalhist") {
		result = colorTf.HistogramEqualization(srcImg, destImg);
	}
	else if (Command == "--drawhist") {
		result = colorTf.DrawHistogram(srcImg, destImg);
	}
	imshow("source image", srcImg);
	if (result)
		imshow("destination image", destImg);
	imwrite("gray.jpeg", destImg);
	cout << destImg.type() << endl;
	
	waitKey();
	return 0;
}