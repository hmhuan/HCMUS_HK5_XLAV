#include "GeometricTransformer.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	//srcImg: anh nguon - dstImg: anh dich
	Mat srcImg, dstImg;
	//Khởi tạo lớp GeometricTransformer
	GeometricTransformer GTf;
	PixelInterpolate *pixInterpolate;
	
	string Command(argv[1]), Interpolate(argv[2]), fileImg(argv[3]);

	int result = 0; //biến lưu kết quả trả về sau khi xử lý

	srcImg = imread(fileImg, -1);

	if (Command == "--zoom") {
		float factor = atof(argv[4]); //Factor là hệ số zoom
		if (Interpolate == "--bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GTf.Scale(srcImg, dstImg, factor, factor, pixInterpolate);
		}
		else
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GTf.Scale(srcImg, dstImg, factor, factor, pixInterpolate);
		}
	}
	else if (Command == "--rotate") {
		float angle = atof(argv[4]); //angle là góc xoay
		if (Interpolate == "--bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GTf.RotateKeepImage(srcImg, dstImg, angle, pixInterpolate);
		}
		else
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GTf.RotateKeepImage(srcImg, dstImg, angle, pixInterpolate);
		}
	}
	else if (Command == "--rotateN") {
		float angle = atof(argv[4]); //angle là góc quay
		if (Interpolate == "--bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GTf.RotateUnkeepImage(srcImg, dstImg, angle, pixInterpolate);
		}
		else
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GTf.RotateUnkeepImage(srcImg, dstImg, angle, pixInterpolate);
		}
	}
	Mat temp;
	
	imshow("Source Image", srcImg);
	cout << srcImg.cols << " " << srcImg.rows << endl;
	if (result)
	{
		imshow("Destination Image", dstImg);
		cout << dstImg.cols << " " << dstImg.rows << endl;
	}

	waitKey(0);
	return 0;
}