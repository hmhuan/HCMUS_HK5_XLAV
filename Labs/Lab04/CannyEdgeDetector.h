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

	// Hàm đệ quy phân ngưỡng
	void HysteresisThresholding(const Mat& G, Mat& dstImage, int x, int y)
	{
		// Kiểm tra xem x, y có vượt quá giới hạn không
		if (x < 0 || y < 0 || x >= dstImage.rows || y >= dstImage.cols || dstImage.at<uchar>(x, y) != 0)
			return;
		//G[x][y] đã bị loại bỏ hay dstImage[x][y] đã được đi qua
		if (G.at<float>(x, y) < _lowThreshold )
			return;
		dstImage.at<uchar>(x, y) = 255;
		// Hysteresis Threshold theo 8 hướng (lân cận 8)
		HysteresisThresholding(G, dstImage, x - 1, y - 1);
		HysteresisThresholding(G, dstImage, x - 1, y);
		HysteresisThresholding(G, dstImage, x - 1, y + 1);
		HysteresisThresholding(G, dstImage, x , y - 1);
		HysteresisThresholding(G, dstImage, x , y + 1);
		HysteresisThresholding(G, dstImage, x + 1, y - 1);
		HysteresisThresholding(G, dstImage, x + 1, y);
		HysteresisThresholding(G, dstImage, x + 1, y + 1);
	}

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
		if (srcImage.empty())
			return 0;

		Mat Gray, Gx, Gy, G, nms; //nms: non-maximal suppression matrix
		vector<int> offsets;
		vector<float> kernelX, kernelY;
		const float BYTE_TO_FLOAT = 1.0f;
		float sum, sumX, sumY;
		
	//0. chuyển sang ảnh xám và lọc gauss
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, Gray, COLOR_BGR2GRAY);
		else
			srcImage.convertTo(Gray, CV_8UC1);
		GaussianBlur(Gray, Gray, Size(5, 5), 0.65f);
		//Khởi tạo ảnh đích có kích thước và type giống ảnh Gray
		dstImage = Mat::zeros(Gray.rows, Gray.cols, CV_8UC1);

		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = dstImage.cols, height = dstImage.rows;
		//nChannels là số kênh màu
		int nChannels = dstImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = dstImage.step[0];
		// Khởi tạo dx, dy
		int dx[9] = {-1,-1,-1,0,0,0,1,1,1 };
		int dy[9] = {-1,0,1,-1,0,1,-1,0,1};
		
		// Khởi tạo offsets
		int kHalfWidth = 3 >> 1, kHalfHeight = 3 >> 1, n = 9;
		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				offsets.push_back(y * widthStep + x);

		uchar *pGray = (uchar *)Gray.data, *pGrayRow;
		uchar *pData = (uchar *)dstImage.data, *pRow;
	//1. Tính đạo hàm theo x, y của ảnh xám , Find Magnitude
		//Sobel operator: để tính đạo hàm theo x, y
		float sobel = 1.0f;
		float Wx[9] = { -1.0 / sobel, 0.0 / sobel, 1.0 / sobel,
						-2.0 / sobel, 0.0 / sobel, 2.0 / sobel,
						-1.0 / sobel, 0.0 / sobel, 1.0 / sobel };
		float Wy[9] = { 1.0 / sobel, 2.0 / sobel, 1.0 / sobel,
						0.0 / sobel, 0.0 / sobel, 0.0 / sobel,
						-1.0 / sobel, -2.0 / sobel, -1.0 / sobel };
		//Khởi tạo 3 ma trận đạo hàm theo x, theo y và gradient với bộ lọc sobel
		Gx.create(height, width, CV_32FC1);
		Gy.create(height, width, CV_32FC1);
		G.create(height, width, CV_32FC1);
		nms.create(height, width, Gray.type()); //nms là ma trận đánh dấu
		// Tính đạo hàm theo hướng x, y và độ lớn gradiant ảnh
		for (int i = 0; i < height; i++, pGray += widthStep)
		{
			pGrayRow = pGray;
			for (int j = 0; j < width; j++, pGrayRow += nChannels)
			{
				sumX = sumY = 0.0f;
				for (int k = 0; k < n; k++)
				{
					sumX += pGrayRow[offsets[k]] * Wx[n - 1 - k];
					sumY += pGrayRow[offsets[k]] * Wy[n - 1 - k];
				}
				sum = hypot(sumX, sumY);
				Gx.at<float>(i, j) = sumX;
				Gy.at<float>(i, j) = sumY;
				G.at<float>(i, j) = sum;
			}
		}
	//2. Find Orientation at each pixel and Non-maximum suppression
		float X, Y, Theta, Gp, Ga, Gb;
		uchar * pNms = (uchar *)nms.data, *pNmsRow;
		for (int i = 0; i < height; i++, pNms += widthStep)
		{
			pNmsRow = pNms;
			for (int j = 0; j < width; j++, pNmsRow += nChannels)
			{
				X = Gx.at<float>(i, j); Y = Gy.at<float>(i, j);
				//Tính hướng
				Theta = fastAtan2(Y, X);
				Gp = G.at<float>(i, j);
				Ga = Gb = 128.0;
				// chia làm 8 dir 
				float dir = (float)(fmod(Theta + 180, 180) / 180) * 8;
				if (dir <= 1 || dir > 7)
				{
					//(x, y-1) (x, y+1)
					if (j - 1 >= 0)
						Ga = G.at<float>(i, j - 1);
					if (j + 1 < width)
						Gb = G.at<float>(i, j + 1);
				}
				else if (dir > 1 && dir <= 3)
				{
					//(x - 1, y + 1), (x + 1, y - 1)
					if (j + 1 < width && i - 1 >= 0)
						Ga = G.at<float>(i - 1, j + 1);
					if (j - 1 >= 0 && i + 1 < height)
						Gb = G.at<float>(i + 1, j - 1);
				}
				else if (dir > 3 && dir <= 5)
				{
					//(x-1, y), (x+1, y)
					if (i - 1 >= 0)
						Ga = G.at<float>(i - 1, j);
					if (i + 1 < height)
						Gb = G.at<float>(i + 1, j);
				}
				else if (dir > 5 && dir <= 7)
				{
					//(x - 1, y - 1), (x + 1, y + 1)
					if (j - 1 >= 0 && i - 1 >= 0)
						Ga = G.at<float>(i - 1, j - 1);
					if (j + 1 < width && i + 1 < height)
						Gb = G.at<float>(i + 1, j + 1);
				}
				//Non-maximal suppression
				//đánh dấu các điểm loại nếu điểm đang xét nhỏ hơn một trong 2 điểm hướng
				if (Gp < Ga || Gp < Gb)
					pNmsRow[0] = 0;
				else
					pNmsRow[0] = 1;
			}
		}
	//3. Loại bỏ điểm không phải biên
		pNms = (uchar *)nms.data;
		for (int i = 0; i < height; i++, pNms += widthStep)
		{
			pNmsRow = pNms;
			for (int j = 0; j < width; j++, pNmsRow += nChannels)
			{
				if (pNmsRow[0] == 0)
					G.at<float>(i, j) = 0;
			}
		}

	//4. Hysteresis Thresholding  [L, H] Recursive
		int * edges = new int[height * width];
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
					
				Gp = G.at<float>(i, j);
				if (Gp >= _highThreshold && dstImage.at<uchar>(i, j) == 0)
				{
					//Recursive
					HysteresisThresholding(G, dstImage, i, j);

					//non-Recursive
					/*int nEdges = 1, i1, j1;
					edges[0] = i * widthStep + j;

					do
					{
						nEdges--;
						const int t = edges[nEdges];

						for (int k = 0; k < 9; k++)
						{
							i1 = t / widthStep + dx[k]; j1 = t % widthStep + dy[k];
							if (i1 >= 0 && i1 < height && j1 >= 0 && j1 < width
							&& G.at<float>(i1, j1) >= _lowThreshold && dstImage.at<uchar>(i1, j1) == 0)
							{
								dstImage.at<uchar>(i1, j1) = 255;
								edges[nEdges] = i1 * widthStep + j1;
								nEdges++;
							}
						}
					} while (nEdges > 0);*/
				}
			}
		}

		// release các Mat đã sử dụng để giải phóng bộ nhớ
		Gray.release();
		G.release();
		Gx.release();
		Gy.release();
		nms.release();
		
		if (dstImage.empty())
			return 0;
		return 1;
	}
	
	void setThreshold(int low, int high)
	{
		_lowThreshold = low;
		_highThreshold = high;
	}

	CannyEdgeDetector()
	{
		_lowThreshold = _highThreshold = 20;
	}
	~CannyEdgeDetector() 
	{

	}
};


