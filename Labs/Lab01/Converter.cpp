#include "Converter.h"

int Converter::RGB2GrayScale(const Mat & sourceImage, Mat & destinationImage)
{	
	uchar lookup[256];
	for (int i = 0; i < 256; i++)
		lookup[i] = saturate_cast<uchar>(i);

	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0]; //so buoc cho anh nguon

	destinationImage.create(height, width, CV_8UC1);
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pData = (uchar*)destinationImage.data; // con tro chay cho anh dich
	uchar* psData = (uchar*)sourceImage.data; // con tro chay anh nguon
	//
	float Y;
	for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0]) {
		uchar * pRow = pData;
		uchar * psRow = psData;
		for (int j = 0; j < width; j++, psRow += nChannels, pRow += 1) {
			//Y ← 0.299⋅R+0.587⋅G+0.114⋅B
			Y = 0.299 * (float)psRow[2] + 0.587 * (float)psRow[1]; + 0.114 * (float)psRow[0];
			pRow[0] = lookup[(int)Y];
		}
	}
	return 1;
}

int Converter::GrayScale2RGB(const Mat & sourceImage, Mat & destinationImage)
{

	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0]; //so buoc cho anh nguon

	destinationImage.create(height, width, CV_8UC3);
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pData = (uchar*)destinationImage.data; // con tro chay cho anh dich
	uchar* psData = (uchar*)sourceImage.data; // con tro chay anh nguon

	float Y;
	for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0]) {
		uchar * pRow = pData;
		uchar * psRow = psData;
		for (int j = 0; j < width; j++, psRow += nChannels, pRow += 3) {
			
			pRow[0] = pRow[1] = pRow[2] = psRow[0];
		}
	}
	return 1;
}

int Converter::RGB2HSV(const Mat & sourceImage, Mat & destinationImage)
{

	return 0;
}

int Converter::HSV2RGB(const Mat & sourceImage, Mat & destinationImage)
{
	return 0;
}

int Converter::Convert(Mat & sourceImage, Mat & destinationImage, int type)
{
	switch (type) {
	case 0:
		return RGB2GrayScale(sourceImage, destinationImage);
	case 1:
		return GrayScale2RGB(sourceImage, destinationImage);
	case 2:
		return RGB2HSV(sourceImage, destinationImage);
	case 3:
		return HSV2RGB(sourceImage, destinationImage);
	}
	return 0;
}

int Converter::Convert(IplImage * sourceImage, IplImage * destinationImage, int type)
{

	return 0;
}

Converter::Converter()
{
}

Converter::~Converter()
{
}
