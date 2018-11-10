#pragma once
#include "Convolution.h"

void MySort(uchar a[],int n)
{
	for(int i=0;i< n/ 2 + 1; i++)
		for (int j = i + 1; j < n; j++)
			if (a[i] > a[j])
			{
				a[i] ^= a[j];
				a[j] ^= a[i];
				a[i] ^= a[j];
			}
}

class Blur
{
public:
	/*
	Hàm làm trơn ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	0: Trung bình
	1: Trung vị
	2: Gaussian	
	Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) 
	{
		Convolution conv;
		vector<float> kernel;
		vector<int> offsets;
		uchar * I;

		int width, height, kHalfWidth, kHalfHeight, nChannels, widthStep;
		uchar * pData, *psData, *pRow, *psRow;
		int n;
		
		switch (method)
		{
		case 0:
			//Tạo kernel với trường hợp lọc trung bình
			n = kWidth * kHeight;
			for (int i = 0; i < kWidth * kHeight; i++)
				kernel.push_back(1.0f/n);
			conv.SetKernel(kernel, kWidth, kHeight);
			return conv.DoConvolution(sourceImage, destinationImage);
			break;
		case 1:
			//xử lý lọc trung vị
			destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());
			//width là chiều rộng ảnh, height là chiều cao ảnh
			width = sourceImage.cols, height = sourceImage.rows;
			//nChannels là số kênh màu
			nChannels = sourceImage.channels();
			//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
			widthStep = sourceImage.step[0];


			//Tạo bảng offsets
			kHalfWidth = kWidth >> 1;
			kHalfHeight = kHeight >> 1;

			for (int y = -kHalfHeight; y <= kHalfHeight; y++)
				for (int x = -kHalfWidth; x <= kHalfWidth; x++)
					offsets.push_back(y*widthStep + x);
			n = offsets.size();

			I = new uchar[n];
			pData = (uchar *)destinationImage.data;
			psData = (uchar *)sourceImage.data;
			for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0]) 
			{
				pRow = pData;
				psRow = psData;
				for (int j = 0; j < width; j++, psRow += nChannels, pRow += 1) 
				{
					//Tìm trung vị của các điểm ảnh theo kSize
					for (int k = 0; k < n; k++)
					{
						I[k] = psRow[offsets[k]];
					}
					MySort(I, n);
					// Gán giá trị điểm ảnh là giá trị trung vị tìm được
					pRow[0] = I[n / 2];
				}
			}
			break;
		case 2:
			//Tạo kernel với trường hợp lọc Gaussian
			conv.SetKernel(kernel, kWidth, kHeight);
			return conv.DoConvolution(sourceImage, destinationImage);
			break;
		default:
			break;
		}
		
	};
	Blur() {};
	~Blur() {};
};

