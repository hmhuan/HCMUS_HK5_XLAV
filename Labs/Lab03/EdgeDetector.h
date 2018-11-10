#pragma once
#include "Convolution.h"


class EdgeDetector
{
public:
	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel	
		1: Sobel
		2: Prewitt
		3: Laplace		
	Hàm trả về
		0: nếu detect thành công
		1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {
		Convolution conv;
		vector<float> kernelX, kernelY;
		Mat fX, fY;
		float Wx[9] = { 1, 0, -1, 
						1, 0, -1, 
						1, 0, -1 };
		float Wy[9] = {	-1, -1, -1, 
						0, 0, 0, 
						1, 1, 1 };
		float WLap[9] = {1, 1, 1,
						1, -8, 1,
						1, 1, 1};
		destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());
		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = sourceImage.cols, height = sourceImage.rows;
		//nChannels là số kênh màu
		int nChannels = sourceImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = sourceImage.step[0];

		uchar *pxData, *pyData, *pData;
		uchar *pxRow, *pyRow, *pRow;

		float sum, x, y;

		const float BYTE_TO_FLOAT = 1.0f;

		switch (method) 
		{
		case 1: //sobel k = 2
			//Tạo ma trận kernel X, Y
			for (int i = 0; i < 9; i++) 
			{
				if (i == 1 || i == 7)
				{
					Wx[i] = (Wx[i] * 2.0f) / 4.0f;
					Wy[i] = Wy[i] / 4.0f;
				}
				else if (i == 3 || i == 5)
				{
					Wy[i] = (Wy[i] * 2.0f) / 4.0f;
					Wx[i] = Wx[i] / 4.0f;
				}
				else 
				{
					Wx[i] = Wx[i] / 4.0f;
					Wy[i] = Wy[i] / 4.0f;
				}
				kernelX.push_back(Wx[i]);
				kernelY.push_back(Wy[i]);
			}
			fX.create(sourceImage.rows, sourceImage.cols, sourceImage.type());
			fY.create(sourceImage.rows, sourceImage.cols, sourceImage.type());
			
			//Tính ma trận fX
			conv.SetKernel(kernelX, 3, 3);
			conv.DoConvolution(sourceImage, fX);
			//Tính ma trận fY
			conv.SetKernel(kernelY, 3, 3);
			conv.DoConvolution(sourceImage, fY);
			
			pxData = (uchar*)fX.data;
			pyData = (uchar*)fY.data;
			pData = (uchar*)destinationImage.data;
			//
			float sum;
			for (int i = 0; i < height; i++, pxData += widthStep, pyData += widthStep, pData += widthStep) 
			{
				pRow = pData;
				pxRow = pxData;
				pyRow = pyData;
				for (int j = 0; j < width; j++, pxRow += nChannels, pyRow += nChannels, pRow += nChannels) 
				{
					x = pxRow[0] * BYTE_TO_FLOAT;
					y = pyRow[0] * BYTE_TO_FLOAT;
					sum = sqrtf(x * x + y * y);
					pRow[0] = saturate_cast<uchar>(sum);
				}
			}
			break;
		case 2: //prewitt k = 1
			//Tạo ma trận kernel X, Y
			for (int i = 0; i < 9; i++)
			{
				kernelX.push_back(Wx[i] / 3.0f);
				kernelY.push_back(Wy[i] / 3.0f);
			}
			fX.create(sourceImage.rows, sourceImage.cols, sourceImage.type());
			fY.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

			//Tính ma trận fX đạo hàm của ảnh nguồn theo x
			conv.SetKernel(kernelX, 3, 3);
			conv.DoConvolution(sourceImage, fX);
			//Tính ma trận fY đạo hàm của ảnh nguồn theo y
			conv.SetKernel(kernelY, 3, 3);
			conv.DoConvolution(sourceImage, fY);
			
			//Con trỏ data của ma trận fX, fY, đích
			pxData = (uchar*)fX.data;
			pyData = (uchar*)fY.data;
			pData = (uchar*)destinationImage.data;
			
			for (int i = 0; i < height; i++, pxData += widthStep, pyData += widthStep, pData += widthStep) 
			{
				pRow = pData;
				pxRow = pxData;
				pyRow = pyData;
				for (int j = 0; j < width; j++, pxRow += nChannels, pyRow += nChannels, pRow += nChannels) 
				{
					//sum = (float)pxRow[0] +(float)pyRow[0]; //????
					x = pxRow[0] * BYTE_TO_FLOAT;
					y = pyRow[0] * BYTE_TO_FLOAT;
					sum = sqrtf(x * x + y * y);
					pRow[0] = saturate_cast<uchar>(sum);
				}
			}
			break;
		case 3: // Laplace với ma trận WLap
			for (int i = 0; i < 9; i++)
				kernelX.push_back(WLap[i]);
			conv.SetKernel(kernelX, 3, 3);
			conv.DoConvolution(sourceImage, destinationImage); //Tích chập 
			break;
		default:
			break;
		}

		return 1;
	};
	EdgeDetector() {};
	~EdgeDetector() {};
};