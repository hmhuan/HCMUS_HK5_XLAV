﻿#include "ColorTransformer.h"


int ColorTransformer::ChangeBrightness(const Mat & sourceImage, Mat & destinationImage, uchar b)
{
	//Tạo bảng lookup
	uchar lookup[256];
	for (int i = 0; i < 256; i++)
		lookup[i] = saturate_cast<uchar>(i + b);
	
	sourceImage.copyTo(destinationImage);

	int width = destinationImage.cols, height = destinationImage.rows;
	//nChannels là số kênh màu
	int nChannels = destinationImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = destinationImage.step[0];
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pData = (uchar*)destinationImage.data;
	for (int i = 0; i < height; i++, pData += widthStep) {
		uchar * pRow = pData;
		for (int j = 0; j < width; j++, pRow += nChannels) {
			for (int k = 0; k < nChannels; k++)
				pRow[k] = lookup[(int)pRow[k]];
		}
	}
	
	return 1;
}

int ColorTransformer::ChangeContrast(const Mat & sourceImage, Mat & destinationImage, float c)
{
	uchar lookup[256];
	for (int i = 0; i < 256; i++)
		lookup[i] = saturate_cast<uchar>(i * c);
	//copy source vao destination
	sourceImage.copyTo(destinationImage);
	//width la so chieu dai anh <-> so cot
	//height la chieu rong anh <-> so dong
	int width = destinationImage.cols, height = destinationImage.rows;
	//nChannels là số kênh màu
	int nChannels = destinationImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = destinationImage.step[0];
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pData = (uchar*)destinationImage.data;
	for (int i = 0; i < height; i++, pData += widthStep) {
		uchar * pRow = pData;
		for (int j = 0; j < width; j++, pRow += nChannels) {
			for (int k = 0; k < nChannels; k++)
				pRow[k] = lookup[(int)pRow[k]];
		}
	}

	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat & sourceImage, Mat & destinationImage)
{
	return 0;
}

int ColorTransformer::CalcHistogram(const Mat & sourceImage, Mat & histogram)
{
	return 0;
}

int ColorTransformer::DrawHistogram(const Mat & sourceImage, Mat & histImage)
{

	return 0;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
