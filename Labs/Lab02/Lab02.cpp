#include "GeometricTransformer.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	//srcImg: ảnh nguồn - dstImg: ảnh đích
	Mat srcImg, dstImg;
	//Khởi tạo lớp GeometricTransformer và PixelInterpolate
	GeometricTransformer GTf;
	PixelInterpolate *pixInterpolate;
	//Tiến hành nhận Command, kiểu nội suy, và tên đường dẫn file ảnh
	string Command(argv[1]), Interpolate(argv[2]), fileImg(argv[3]);

	int result = 0; //biến lưu kết quả trả về sau khi xử lý
	//Đọc ảnh với type = -1 (nguyên gốc ảnh)
	srcImg = imread(fileImg, -1);

	if (Command == "--zoom") {
		float factorX = atof(argv[4]); //FactorX là hệ số zoom theo x
		float factorY = atof(argv[5]); //FactorY là hệ số zoom theo y
		if (Interpolate == "--bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GTf.Scale(srcImg, dstImg, factorX, factorY, pixInterpolate);
		}
		else
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GTf.Scale(srcImg, dstImg, factorX, factorY, pixInterpolate);
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

	if (result)
	{
		imshow("Source Image", srcImg);
		imshow("Destination Image", dstImg);
	}
	else
		printf("Could not open or find the image");

	waitKey(0);
	return 0;
}
