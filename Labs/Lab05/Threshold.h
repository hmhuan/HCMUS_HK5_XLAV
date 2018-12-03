#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"


using namespace cv;
using namespace std;


void MySort(uchar *&a, int n)
{
	for (int i = 0; i< n / 2 + 1; i++)
		for (int j = i + 1; j < n; j++)
			if (a[i] > a[j])
			{
				a[i] ^= a[j];
				a[j] ^= a[i];
				a[i] ^= a[j];
			}
}

void createIntergral(const Mat& srcImage, Mat &intergral, int k)
{
	//Chiều rộng, chiều cao, số "bước", số kênh ( = 1) của nahr nguồn
	int width = srcImage.cols, height = srcImage.rows;
	int widthStep = srcImage.step[0];
	int nChannels = srcImage.channels();

	//Khởi tạo intergral là ma trận 0
	intergral = Mat::zeros(height + 1, width + 1, CV_32SC1);

	//Con trỏ data, dòng của ma trận nguồn và ma trận intergral
	uchar * psData = (uchar *)srcImage.data, *psRow;
	int * pData = (int *)intergral.data, *pRow;
	pData = pData + widthStep + 2;
	int offsets[3] = { -1, -(widthStep + 1), -(widthStep + 1) - 1 };
	int temp;
	for (int i = 1; i <= height; i++, psData += widthStep, pData += (widthStep + 1))
	{
		pRow = pData;
		psRow = psData;
		for (int j = 1; j <= width; j++, psRow += nChannels, pRow += nChannels)
		{
			temp = 1;
			for (int id = 0; id < k; id++)
				temp *= psRow[0];
			pRow[0] = pRow[offsets[0]] + pRow[offsets[1]] - pRow[offsets[2]] + temp;
		}
	}
}

//phân ngưỡng tĩnh
class StaticThreshold
{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;
public:
	/*
	Hàm áp dụng phân ngưỡng tĩnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả	
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/
	void setThreshold(int low, int high) 
	{
		_lowThreshold = low;
		_highThreshold = high;
	}
	int Apply(const Mat& srcImage, Mat &dstImage)
	{
		if (srcImage.empty())
			return 0;
		//Tạo LookUp Table
		uchar lookUp[256] = { 0 };
		for (int i = _lowThreshold + 1; i < _highThreshold; i++)
			lookUp[i] = 255;
		//Khởi tạo ảnh đích là ảnh nguồn hoặc ảnh xám của ảnh nguồn
		dstImage = srcImage.clone();
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, dstImage, COLOR_BGR2GRAY);
		else
			dstImage = srcImage.clone();
		
		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = dstImage.cols, height = dstImage.rows;
		//nChannels là số kênh màu
		int nChannels = dstImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = dstImage.step[0];
		//Con trỏ data và con trỏ dòng của ảnh
		uchar *pData = (uchar *)dstImage.data, *pRow;

		for (int i = 0; i < height; i++, pData += widthStep)
		{
			pRow = pData;
			for (int j = 0; j < width; j++, pRow += nChannels)
			{
				pRow[0] = lookUp[(int)pRow[0]];
			}
		}
		if (dstImage.empty())
			return 0;
		return 1;
	}

	StaticThreshold() {}
	~StaticThreshold() {}
};


//phân ngưỡng cục bộ dựa vào trung bình
class AverageLocalThreshold
{
	//hệ số C
	int _C;

public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ theo trung bình
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
		1: nếu phân ngưỡng thành công
		0: nếu phân ngưỡng không thành công
	*/
	void setC(int C)
	{
		_C = C;
	}
	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
	{
		Mat Intergral, Gray;
		//Khởi tạo ảnh đích là ảnh xám
		if (srcImage.empty())
			return 0;
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, Gray, COLOR_BGR2GRAY);
		else
			Gray = srcImage.clone();
		dstImage.create(Gray.rows, Gray.cols, Gray.type());
		//Tạo ma trận ảnh Intergral
		createIntergral(Gray, Intergral, 1);

		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = dstImage.cols, height = dstImage.rows;
		//nChannels là số kênh màu
		int nChannels = dstImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = dstImage.step[0];

		//Tạo bảng offsets
		int kHalfWidth = winSize.width >> 1;
		int kHalfHeight = winSize.height >> 1;
		vector<int> offsets;
		int n = winSize.width * winSize.height;
		float average;
		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				offsets.push_back(y * (widthStep) + x);
		int TopBot[4];
		TopBot[0] = offsets[0];// +(-(widthStep + 1) - 1); //TL
		TopBot[1] = offsets[winSize.width - 1];// +(-(widthStep + 1)); //TR
		TopBot[2] = offsets[n - winSize.width];// +(-1); //BL
		TopBot[3] = offsets[n - 1]; //BR

		//Con trỏ data và con trỏ dòng của ảnh
		uchar *pData = (uchar *)dstImage.data, *pRow;
		uchar *psData = (uchar *)Gray.data, *psRow;
		int *pI = (int *)Intergral.data, *pIRow;
		// dịch chuyển con trỏ data của  intergral tới ô (1, 1) ứng với ma trận
		pI = pI + (widthStep + 1) + 1;
		
		//0: TopLeft, 1: TopRight, 2: BotLeft, 3: BotRight
		//int TL = -kHalfHeight  * widthStep - kHalfWidth  + (-widthStep - 1);
		//int TR = -kHalfHeight * widthStep + kHalfWidth + (-widthStep);
		//int BL = kHalfHeight * widthStep - kHalfWidth + (-1);
		//int BR = kHalfHeight * (widthStep + 1) + kHalfWidth;

		for (int i = 0; i < height; i++, pData += widthStep, psData += widthStep, pI += (widthStep + 1))
		{
			pRow = pData;
			psRow = psData;
			pIRow = pI;
			for (int j = 0; j < width; j++, pRow += nChannels, psRow += nChannels, pIRow += nChannels)
			{
				average = 0.0f;
				/*for (int k = 0; k < 4; k++) {
					if ((i * j + TopBot[k]) >= -widthStep * 2 && (i * j + TopBot[k]) <= width * height) {
						if (k == 0 || k == 3)
							average += pIRow[TopBot[k]];
						else
							average -= pIRow[TopBot[k]];
					}

				}*/
				for (int k = 0; k < n; k++)
					average += psRow[offsets[k]];
				average /= n;
				pRow[0] = psRow[0] > (average - _C) ? 255 : 0;
			}
		}

		Gray.release();
		if (dstImage.empty())
			return 0;
		return 1;
	}
	AverageLocalThreshold() { _C = 0; }
	~AverageLocalThreshold() {}
};

//phân ngưỡng cục bộ dựa vào trung vị
class MedianLocalThreshold
{
	//hệ số C
	int _C;

public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ dựa vào trung vị
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/
	void setC(int C)
	{
		_C = C;
	}
	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
	{
		Mat Gray;
		//Khởi tạo ảnh đích là ảnh xám
		if (srcImage.empty())
			return 0;
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, Gray, COLOR_BGR2GRAY);
		else
			Gray = srcImage.clone();
		
		dstImage.create(Gray.rows, Gray.cols, Gray.type());

		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = dstImage.cols, height = dstImage.rows;
		//nChannels là số kênh màu
		int nChannels = dstImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = dstImage.step[0];
		//Con trỏ data và con trỏ dòng của ảnh
		uchar *pData = (uchar *)dstImage.data, *pRow;
		uchar *psData = (uchar *)Gray.data, *psRow;

		//Tạo bảng offsets
		int kHalfWidth = winSize.width >> 1;
		int kHalfHeight = winSize.height >> 1;
		vector<int> offsets;
		uchar *I;
		int n = winSize.width * winSize.height;

		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				offsets.push_back(y * widthStep + x);
		//Mảng để lưu các phần tử trong kernel tìm trung vị
		I = new uchar[n];
		int x = 0, y = 0;
		for (x = 0; x < height; x++, pData += widthStep, psData += widthStep)
		{
			psRow = psData;
			pRow = pData;
			for (y = 0; y < width; y++, pRow += nChannels, psRow += nChannels)
			{
				//Tìm trung vị của các điểm ảnh theo kSize
				for (int k = 0; k < n; k++)
				{
					I[k] = psRow[offsets[k]];
				}
				MySort(I, n);
				// Gán giá trị điểm ảnh là giá trị trung vị tìm được
				pRow[0] = psRow[0] > ((int)I[n / 2] - _C) ? 255 : 0;
			}
		}
		Gray.release();
		delete[] I;
		if (dstImage.empty())
			return 0;
		return 1;
	}

	MedianLocalThreshold()
	{
		_C = 0;
	}
	~MedianLocalThreshold() {}
};

//phân ngưỡng cục bộ dựa vào thuật toán Sauvola
class SauvolaLocalThreshold
{
	//hệ số r
	int _r;
	//hệ số k
	float _k;
public:
	/*
	Hàm áp dụng thuật toán Sauvola để phân ngưỡng
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/
	void setSauvola(float k, int r)
	{
		_k = k;
		_r = r;
	}
	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize) 
	{
		Mat Intergral, SqIntergral, Gray;
		const float BYTE_TO_FLOAT = 1.0f;
		//Khởi tạo ảnh đích là ảnh xám
		if (srcImage.empty())
			return 0;
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, Gray, COLOR_BGR2GRAY);
		else
			Gray = srcImage.clone();
		dstImage.create(Gray.rows, Gray.cols, Gray.type());

		//Tạo ma trận ảnh Intergral và SquareIntergral
		//createIntergral(Gray, Intergral, 1);
		//createIntergral(Gray, SqIntergral, 2);
		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = dstImage.cols, height = dstImage.rows;
		//nChannels là số kênh màu
		int nChannels = dstImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = dstImage.step[0];
		//Tạo bảng offsets
		int kHalfWidth = winSize.width >> 1;
		int kHalfHeight = winSize.height >> 1;
		vector<int> offsets;
		int n = winSize.width * winSize.height;
		float average, standard_deviation;
		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				offsets.push_back(y * (widthStep) + x);

		//Con trỏ data và con trỏ dòng của ảnh
		uchar *pData = (uchar *)dstImage.data, *pRow;
		uchar *psData = (uchar *)Gray.data, *psRow;
		//int *pI = (int *)Intergral.data, *pIRow;
		// dịch chuyển con trỏ data của  intergral tới ô (1, 1) ứng với ma trận
		///pI = pI + (widthStep + 1) + 1;

		//0: TopLeft, 1: TopRight, 2: BotLeft, 3: BotRight
		//int TL = -kHalfHeight  * widthStep - kHalfWidth  + (-widthStep - 1);
		//int TR = -kHalfHeight * widthStep + kHalfWidth + (-widthStep);
		//int BL = kHalfHeight * widthStep - kHalfWidth + (-1);
		//int BR = kHalfHeight * (widthStep + 1) + kHalfWidth;

		for (int i = 0; i < height; i++, pData += widthStep, psData += widthStep)
		{
			pRow = pData;
			psRow = psData;
			for (int j = 0; j < width; j++, pRow += nChannels, psRow += nChannels)
			{
				average = 0.0f; standard_deviation = 0.0f;
				for (int k = 0; k < n; k++)
					average += psRow[offsets[k]] ;
				average /= n;
				for (int k = 0; k < n; k++)
				{
					float temp = psRow[offsets[k]] * BYTE_TO_FLOAT;
					standard_deviation += (temp - average) * (temp - average);
				}
				standard_deviation = sqrtf(standard_deviation / (n - 1));
				pRow[0] = psRow[0] > average * (1 + _k*(standard_deviation / _r - 1)) ? 255 : 0;
			}
		}

		Gray.release();
		if (dstImage.empty())
			return 0;
		return 1;
	}


	SauvolaLocalThreshold() 
	{ 
		_k = 0.5; 
		_r = 128;
	}
	~SauvolaLocalThreshold() {}
};