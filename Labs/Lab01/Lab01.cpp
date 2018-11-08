#include "ColorTransformer.h"
#include "Converter.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char * argv[]) {
	
	Mat srcImg, destImg;
	Mat hsv;
	int result = -1; // Kết quả có thực hiện được không 
	Converter converter; //Khởi tạo lớp Converter
	ColorTransformer colorTf; //Khởi tạo lớp ColorTransfer
	
	string Command = string(argv[1]);
	//???
	srcImg = imread(argv[2]);

	if (Command == "--rgb2gray") {
		//srcImg = imread(argv[2]);
		//if (!srcImg.empty())
			result = converter.Convert(srcImg, destImg, 0);
	}
	else if (Command == "--gray2rgb") {
		
		//if (!srcImg.empty())
			result = converter.Convert(srcImg, destImg, 1);
	}
	else if (Command == "--rgb2hsv") {
		result = converter.Convert(srcImg, destImg, 2);
	}
	else if (Command == "--hsv2rgb") {
		result = converter.Convert(srcImg, destImg, 3);
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
	cvtColor(srcImg, hsv, CV_HSV2BGR);
	imshow("BGR", hsv);
	if (result)
		imshow("destination image", destImg);
	imwrite("test1.png", destImg);
	
	cout << srcImg.type() <<" " <<srcImg.depth() << " " << srcImg.channels() << "\n";
	cout << destImg.type() << " " << destImg.depth() << " " << destImg.channels() << "\n";
	waitKey();
	return 0;
}