#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"


using namespace cv;
using namespace std;

class Convolution
{
	vector<float> _kernel;// lưu giá trị của ma trận kernel dùng để tích chập
	int _kernelWidth;//chiều rộng kernel
	int _kernelHeight;//chiều dài kernel
public:
	//trả về kernel 
	vector<float> GetKernel() { return _kernel; };
	//set kernel, chiều rộng, chiều dài kernel phải là số lẻ
	void SetKernel(vector<float> kernel, int kWidth, int kHeight) {
		_kernel = kernel;
		_kernelWidth = kWidth;
		_kernelHeight = kHeight;
	};

	/*
	Hàm tính convolution của 1 ảnh xám với kernel được xác định trước
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu tính thành công
	1: nếu tính thất bại (không đọc được ảnh input,...)
	*/
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage) 
	{

		uchar lookup[256];
		for (int i = 0; i < 256; i++)
			lookup[i] = (uchar)i;
		//Kiểm tra ma trận nguồn
		if (sourceImage.empty())
			return 0;

		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = sourceImage.cols, height = sourceImage.rows;
		//nChannels là số kênh màu
		int nChannels = sourceImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = sourceImage.step[0];

		//Tạo ảnh đích với kích thước ảnh nguồn và type là ảnh grayscale
		destinationImage.create(height, width, CV_8UC1);

		//pData là con trỏ quản lý vùng nhớ ảnh
		uchar* pData = (uchar*)destinationImage.data; // con trỏ data ảnh đích
		uchar* psData = (uchar*)sourceImage.data; // con trỏ data ảnh nguồn
									
		//Tạo bảng offsets
		int kHalfWidth = _kernelWidth >> 1;
		int kHalfHeight = _kernelHeight >> 1;

		vector<int> offsets;
		int n;
		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				offsets.push_back(y*widthStep + x);
		n = offsets.size();

		float sum;
		for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0]) {
			uchar * pRow = pData;
			uchar * psRow = psData;
			for (int j = 0; j < width; j++, psRow += nChannels, pRow += 1) {
				sum = 0.0f;
				//Tính tích chập
				for (int k = 0; k < n; k++)
					sum += psRow[offsets[k]] * _kernel[n - 1 - k];
				pRow[0] = lookup[(int)sum];//saturate_cast<uchar>(sum);
			}
		}

		return 1;

	};
	Convolution() {};
	~Convolution() {};
};

