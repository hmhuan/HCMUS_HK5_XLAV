#include "GeometricTransformer.h"


PixelInterpolate::PixelInterpolate()
{
}

PixelInterpolate::~PixelInterpolate()
{
}

//Các hàm của lớp BilinearInterpolate

void BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels, uchar * pDstRow)
{
}

BilinearInterpolate::BilinearInterpolate()
{
}

BilinearInterpolate::~BilinearInterpolate()
{
}

//Các hàm của lớp NearestNeighborInterpolate

void NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels, uchar * pDstRow)
{

}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{
}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{
}

//Các hàm của lớp AffineTransform

void AffineTransform::Translate(float dx, float dy)
{
	//Gán các giá trị dx, dy tại (0, 2), (1, 2)
	_matrixTransform.at<float>(0, 2) = dx;
	_matrixTransform.at<float>(1, 2) = dy;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			std::cout << _matrixTransform.at<float>(i, j) << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}

void AffineTransform::Rotate(float angle)
{
	//Tính sinA, cosA
	angle = angle * M_PI / 180;
	float sinAlpha = sinf(angle), cosAlpha = cosf(angle);
	//Gán các giá trị cosA, -sinA, sinA, cosA  tại (0, 0), (0, 1), (1, 0), (1, 1)
	_matrixTransform.at<float>(0, 0) = cosAlpha;
	_matrixTransform.at<float>(0, 1) = -sinAlpha;
	_matrixTransform.at<float>(1, 0) = sinAlpha;
	_matrixTransform.at<float>(1, 1) = cosAlpha;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			std::cout << _matrixTransform.at<float>(i, j) << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}

void AffineTransform::Scale(float sx, float sy)
{
	//Gán các giá trị sx, sy tại (0, 0), (1, 1)
	_matrixTransform.at<float>(0, 0) = sx;
	_matrixTransform.at<float>(1, 1) = sy;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			std::cout << _matrixTransform.at<float>(i, j) << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}
		

void AffineTransform::TransformPoint(float & x, float & y)
{
	float P[3] = { x, y, 1 }, P1[3] = {0, 0, 0}; //Tạo 2 ma trận điểm ảnh nguồn và đích
	//Nhân ma trận nguồn với ma trận chuyển đổi
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			P1[i] += P[j] * _matrixTransform.at<float>(i, j);
	//Gán lại giá trị mới cho điểm ảnh
	x = P1[0];
	y = P1[1];
}

AffineTransform::AffineTransform()
{
	//Khởi tạo matrix 3x3 type 32F chanels = 1;
	_matrixTransform.create(3, 3, CV_32FC1);
	//Khởi tạo matrix 3x3 với [1, 0, 0]
	//						  [0, 1, 0]
	//						  [0, 0, 1]	
	int width = _matrixTransform.cols, height = _matrixTransform.rows;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (x == y)
				_matrixTransform.at<float>(x, y) = 1;
			else
				_matrixTransform.at<float>(x, y) = 0;
}

AffineTransform::~AffineTransform()
{
}


//Các hàm của lớp GeometricTransformer

int GeometricTransformer::Transform(const Mat & beforeImage, Mat & afterImage, AffineTransform * transformer, PixelInterpolate * interpolator)
{
	return 0;
}

int GeometricTransformer::RotateKeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	return 0;
}

int GeometricTransformer::RotateUnkeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	return 0;
}

int GeometricTransformer::Scale(const Mat & srcImage, Mat & dstImage, float sx, float sy, PixelInterpolate * interpolator)
{

	return 0;
}

GeometricTransformer::GeometricTransformer()
{
}

GeometricTransformer::~GeometricTransformer()
{
}
