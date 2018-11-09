#include "ColorTransformer.h"

int ColorTransformer::ChangeBrightness(const Mat &sourceImage, Mat &destinationImage, uchar b)
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
	uchar *pData = (uchar *)destinationImage.data;
	for (int i = 0; i < height; i++, pData += widthStep)
	{
		uchar *pRow = pData;
		for (int j = 0; j < width; j++, pRow += nChannels)
		{
			for (int k = 0; k < nChannels; k++)
				pRow[k] = lookup[(int)pRow[k]];
		}
	}

	return 1;
}

int ColorTransformer::ChangeContrast(const Mat &sourceImage, Mat &destinationImage, float c)
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
	uchar *pData = (uchar *)destinationImage.data;
	for (int i = 0; i < height; i++, pData += widthStep)
	{
		uchar *pRow = pData;
		for (int j = 0; j < width; j++, pRow += nChannels)
		{
			for (int k = 0; k < nChannels; k++)
				pRow[k] = lookup[(int)pRow[k]];
		}
	}

	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat &sourceImage, Mat &destinationImage)
{
	return 0;
}

int ColorTransformer::CalcHistogram(const Mat &sourceImage, Mat &histogram)
{
	histogram = Mat::zeros(3, 256, CV_32S);

	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0];
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar *pData = (uchar *)sourceImage.data;
	for (int i = 0; i < height; i++, pData += widthStep)
	{
		uchar *pRow = pData;
		for (int j = 0; j < width; j++, pRow += nChannels)
		{
			histogram.at<signed>(0, pRow[0])++;
			histogram.at<signed>(1, pRow[1])++;
			histogram.at<signed>(2, pRow[2])++;
		}
	}

	// for (int i = 0; i < histogram.cols; i++)
	// {
	// 	printf("%d %d %d\t", histogram.at<signed>(0, i), histogram.at<signed>(1, i), histogram.at<signed>(2, i));
	// }
	// cout << endl;

	return 1;
}

int ColorTransformer::DrawHistogram(const Mat &sourceImage, Mat &histImage)
{
	Mat histogram;
	histImage = Mat::zeros(480, 640, CV_8UC3);
	signed max_b = 0, max_g = 0, max_r = 0;
	float coeff[3];
	uchar color[3][3] = {255, 0, 0, 0, 255, 0, 0, 0, 255};

	CalcHistogram(sourceImage, histogram);

	for (int i = 0; i < histogram.cols; i++)
	{
		max_b = max(max_b, histogram.at<signed>(0, i));
		max_g = max(max_g, histogram.at<signed>(1, i));
		max_r = max(max_r, histogram.at<signed>(2, i));
	}

	coeff[0] = 480.0 / max_b;
	coeff[1] = 480.0 / max_g;
	coeff[2] = 480.0 / max_r;

	for (int chanel = 0; chanel < histogram.rows; chanel++)
	{
		float binWidth = float(histImage.cols) / (256 - 1);
		float lastX = 0, lastY = histImage.rows;

		for (int bin = 0; bin < histogram.cols; ++bin)
		{
			float x = binWidth * bin;
			float y = histImage.rows - histogram.at<signed>(chanel, bin) * coeff[chanel];
			Vec3b color3b = Vec3b(color[chanel][0], color[chanel][1], color[chanel][2]);

			if (x > 0 && y > 0)
			{
				//circle(histImage, Point(x, y), 2, color3b, -1);
				line(histImage, Point(lastX, lastY), Point(x, y), color3b, 1, 8);
				lastX = x;
				lastY = y;
			}
		}
	}

	return 1;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
