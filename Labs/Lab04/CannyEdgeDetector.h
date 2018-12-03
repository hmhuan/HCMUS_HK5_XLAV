#pragma once
#define _USE_MATH_DEFINES
#include "Convolution.h"
#include <math.h>
#include <vector>

class CannyEdgeDetector
{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;

public:
	/*
		Hàm áp dụng thuật toán Canny để phát hiện biên cạnh
		- srcImage: ảnh input
		- dstImage: ảnh kết quả
		Hàm trả về
			1: nếu detect thành công
			0: nếu detect không thành công
	*/


	int Apply(const Mat& srcImage, Mat &dstImage) 
	{
		Mat Gray, Gx, Gy;
		Convolution conv;
		vector<int> offsets;
		vector<float> kernelX, kernelY;
		const float BYTE_TO_FLOAT = 1.0f;
		int kHalfWidth = 3 >> 1, kHalfHeight = 3 >> 1, n = 9;
	//0. Kiểm tra ảnh nguồn và khởi tạo xảnh xám, ảnh đích, lọc Gauss
		if (srcImage.empty())
			return 0;
		//Khởi tạo ảnh Gray ảnh xám và lọc Gauss kernelSize = 5x5, sigma = 1.4 
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, Gray, COLOR_BGR2GRAY);
		else
			Gray = srcImage.clone();
		GaussianBlur(Gray, Gray, Size(5, 5), 1.4);
		//Khởi tạo ảnh đích có kích thước và type giống ảnh Gray
		dstImage.create(Gray.rows, Gray.cols, Gray.type());

	//1. Tính đạo hàm theo x, y của ảnh xám
		//Sobel operator: để tính đạo hàm theo x, y
		float sobel = 4.0f;
		float Wx[9] = { 1.0/sobel, 0.0/sobel, -1.0/sobel,
						2.0/sobel, 0.0/sobel, -2.0/ sobel,
						1.0/ sobel, 0.0/ sobel, -1.0/ sobel };
		float Wy[9] = { -1.0/ sobel, -2.0/ sobel, -1.0/ sobel,
						0.0/ sobel, 0.0/sobel, 0.0/ sobel,
						1.0/ sobel, 2.0/ sobel, 1.0/ sobel };
		//Tạo ma trận kernel x, kernel y
		for (int i = 0; i < 9; i++)
		{
			kernelX.push_back(Wx[i]);
			kernelY.push_back(Wy[i]);
		}
		Gx.create(Gray.rows, Gray.cols, Gray.type());
		Gy.create(Gray.rows, Gray.cols, Gray.type());

		//Tính ma trận Gx đạo hàm của ảnh xám theo x
		conv.SetKernel(kernelX, 3, 3);
		conv.DoConvolution(Gray, Gx);
		//Tính ma trận GY đạo hàm của ảnh xám theo y
		conv.SetKernel(kernelY, 3, 3);
		conv.DoConvolution(Gray, Gy);

	//2. 
		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = dstImage.cols, height = dstImage.rows;
		//nChannels là số kênh màu
		int nChannels = dstImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = dstImage.step[0];
		////Tạo bảng offsets
		//for (int y = -kHalfHeight; y <= kHalfHeight; y++)
		//	for (int x = -kHalfWidth; x <= kHalfWidth; x++)
		//		offsets.push_back(y * widthStep + x);

		
		//Con trỏ data của ảnh đíc, psData con trỏ ảnh xám Gray
		uchar * pData = (uchar*)dstImage.data, * pRow;
		uchar * pxData = (uchar *)Gx.data, *pxRow;
		uchar * pyData = (uchar *)Gy.data, *pyRow;
		uchar * psData = (uchar*)Gray.data, *psRow;
		
		//G là độ lớn, Theta là góc hướng, X Y: đạo hàm theo hướng x, y
		float G, X, Y, Theta;
		for (int i = 0; i < height; i++, pData += widthStep, pxData += widthStep, pyData += widthStep) 
		{
			pRow = pData;
			pxRow = pxData;
			pyRow = pyData;
			for (int j = 0; j < width; j++, pRow += nChannels, pxRow += nChannels, pyRow += nChannels)
			{
				X = pxRow[0] * BYTE_TO_FLOAT; Y = pyRow[0] * BYTE_TO_FLOAT;
				G = sqrtf(X * X + Y * Y);
				Theta = fastAtan2(Y, X);
				////tobe continued


			}
		}

		if (dstImage.empty())
			return 0;
		imshow("Gray",Gray);
		return 1;
	}

	void setThreshold(int low, int high)
	{
		_lowThreshold = low;
		_highThreshold = high;
	}

	CannyEdgeDetector()
	{

	}
	~CannyEdgeDetector() 
	{

	}
};


