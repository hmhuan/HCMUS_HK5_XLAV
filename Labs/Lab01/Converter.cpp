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
			//Y = 0.299 * (float)psRow[2] + 0.587 * (float)psRow[1]; + 0.114 * (float)psRow[0];
			Y = ((int)psRow[2] * 2 + (int)psRow[1] * 5 + (int)psRow[0] * 1) / 8;
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

	for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0]) {
		uchar * pRow = pData;
		uchar * psRow = psData;
		for (int j = 0; j < width; j++, psRow += nChannels, pRow += 3) {
			pRow[0] = psRow[0];
			pRow[1] = psRow[0];
			pRow[2] = psRow[0];
		}
	}
	return 1;
}

float max(float a, float b, float c) {
	float max = a;
	if (max < b)
		max = b;
	if (max < c)
		max = c;
	return max;
}

float min(float a, float b, float c) {
	//return (a < b ? a : b) < c ? (a < b ? a : b) : c;
	float min = a;
	if (min > b)
		min = b;
	if (min > c)
		min = c;
	return min;
}

int Converter::RGB2HSV(const Mat & sourceImage, Mat & destinationImage)
{
	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0]; //so buoc cho anh nguon
	
	//Khởi tạo ảnh đích với kích thước như ảnh nguồn
	destinationImage.create(height, width, CV_8UC3);
	
	float fR, fG, fB, H, S, Vmax, Vmin;

	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

	uchar* pData = (uchar*)destinationImage.data; // con tro chay cho anh dich
	uchar* psData = (uchar*)sourceImage.data; // con tro chay anh nguon

	for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0]) {
		uchar * pRow = pData;
		uchar * psRow = psData;
		for (int j = 0; j < width; j++, psRow += nChannels, pRow += 3) {
			
			//Chuyen he so sang float trong khoang tu 0.0 -> 1.0
			fB = psRow[0] * BYTE_TO_FLOAT;
			fG = psRow[1] * BYTE_TO_FLOAT;
			fR = psRow[2] * BYTE_TO_FLOAT;

			//Tim V = Vmax, va Vmin
			Vmax = max(fB, fG, fR);
			Vmin = min(fB, fG, fR);
			//Tim S
			S = (Vmax == 0.0f) ? 0 : ((Vmax - Vmin) / Vmax);
			//Tim H
			if (Vmax == fB) {
				H = 240 + 60 * (fR - fG) / (Vmax - Vmin);
			}
			else if (Vmax == fG) {
				H = 120 + 60 * (fB - fR) / (Vmax - Vmin);
			}
			else {
				H = 60 * (fG - fB) / (Vmax - Vmin);
			}
			H = (H < 0) ? H + 360 : H; //Neu H am thi cong them 360 (vi 0 < H < 360 )

			pRow[0] = saturate_cast<uchar>(H / 2);	//H value
			pRow[1] = saturate_cast<uchar>(S * FLOAT_TO_BYTE); //S value
			pRow[2] = saturate_cast<uchar>(Vmax * FLOAT_TO_BYTE); //V value 

 		}
	}
	
	return 1;
}

int Converter::HSV2RGB(const Mat & sourceImage, Mat & destinationImage)
{
	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0]; //so buoc cho anh nguon

										 //Khởi tạo ảnh đích với kích thước như ảnh nguồn
	destinationImage.create(height, width, CV_8UC3);

	float fC, fX, fM, fHbin, fH, fS, fV, fR, fG, fB;

	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

	uchar* pData = (uchar*)destinationImage.data; // con tro chay cho anh dich
	uchar* psData = (uchar*)sourceImage.data; // con tro chay anh nguon

	for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0]) {
		uchar * pRow = pData;
		uchar * psRow = psData;
		for (int j = 0; j < width; j++, psRow += nChannels, pRow += 3) {

			fH = psRow[0] * 2.0;
			fS = psRow[1] * BYTE_TO_FLOAT;
			fV = psRow[2] * BYTE_TO_FLOAT;

			fC = fS * fV;
			fHbin = fmod(fH / 60.0, 6);
			fX = fC * (1 - fabs(fmod(fHbin, 2) - 1));
			fM = fV - fC;

			if (0 <= fHbin && fHbin < 1) {
				fR = fC;
				fG = fX;
				fB = 0;
			}
			else if (1 <= fHbin && fHbin < 2) {
				fR = fX;
				fG = fC;
				fB = 0;
			}
			else if (2 <= fHbin && fHbin < 3) {
				fR = 0;
				fG = fC;
				fB = fX;
			}
			else if (3 <= fHbin && fHbin < 4) {
				fR = 0;
				fG = fX;
				fB = fC;
			}
			else if (4 <= fHbin && fHbin < 5) {
				fR = fX;
				fG = 0;
				fB = fC;
			}
			else if (5 <= fHbin && fHbin < 6) {
				fR = fC;
				fG = 0;
				fB = fX;
			}
			else {
				fR = 0;
				fG = 0;
				fB = 0;
			}

			fR += fM;
			fG += fM;
			fB += fM;

			pRow[2] = saturate_cast<uchar>(fR * FLOAT_TO_BYTE); //R value 
			pRow[1] = saturate_cast<uchar>(fG * FLOAT_TO_BYTE); //G value
			pRow[0] = saturate_cast<uchar>(fB * FLOAT_TO_BYTE);	//B value
		}
	}

	return 1;
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
